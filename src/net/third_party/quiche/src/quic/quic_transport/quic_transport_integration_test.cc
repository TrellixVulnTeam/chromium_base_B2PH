// Copyright (c) 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// An integration test that covers interactions between QuicTransport client and
// server sessions.

#include <memory>
#include <vector>

#include "url/gurl.h"
#include "url/origin.h"
#include "net/third_party/quiche/src/quic/core/crypto/quic_crypto_client_config.h"
#include "net/third_party/quiche/src/quic/core/crypto/quic_crypto_server_config.h"
#include "net/third_party/quiche/src/quic/core/quic_connection.h"
#include "net/third_party/quiche/src/quic/core/quic_error_codes.h"
#include "net/third_party/quiche/src/quic/core/quic_types.h"
#include "net/third_party/quiche/src/quic/core/quic_versions.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_flags.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_test.h"
#include "net/third_party/quiche/src/quic/quic_transport/quic_transport_client_session.h"
#include "net/third_party/quiche/src/quic/quic_transport/quic_transport_server_session.h"
#include "net/third_party/quiche/src/quic/quic_transport/quic_transport_stream.h"
#include "net/third_party/quiche/src/quic/test_tools/crypto_test_utils.h"
#include "net/third_party/quiche/src/quic/test_tools/quic_test_utils.h"
#include "net/third_party/quiche/src/quic/test_tools/quic_transport_test_tools.h"
#include "net/third_party/quiche/src/quic/test_tools/simulator/link.h"
#include "net/third_party/quiche/src/quic/test_tools/simulator/quic_endpoint_base.h"
#include "net/third_party/quiche/src/quic/test_tools/simulator/simulator.h"
#include "net/third_party/quiche/src/quic/test_tools/simulator/switch.h"
#include "net/third_party/quiche/src/quic/tools/quic_transport_simple_server_session.h"

namespace quic {
namespace test {
namespace {

using simulator::QuicEndpointBase;
using simulator::Simulator;
using testing::Assign;

url::Origin GetTestOrigin() {
  constexpr char kTestOrigin[] = "https://test-origin.test";
  GURL origin_url(kTestOrigin);
  return url::Origin::Create(origin_url);
}

ParsedQuicVersionVector GetVersions() {
  return {ParsedQuicVersion{PROTOCOL_TLS1_3, QUIC_VERSION_99}};
}

class QuicTransportEndpointBase : public QuicEndpointBase {
 public:
  QuicTransportEndpointBase(Simulator* simulator,
                            const std::string& name,
                            const std::string& peer_name,
                            Perspective perspective)
      : QuicEndpointBase(simulator, name, peer_name) {
    connection_ = std::make_unique<QuicConnection>(
        TestConnectionId(0x10), simulator::GetAddressFromName(peer_name),
        simulator, simulator->GetAlarmFactory(), &writer_,
        /*owns_writer=*/false, perspective, GetVersions());
    connection_->SetSelfAddress(simulator::GetAddressFromName(name));
  }
};

class QuicTransportClientEndpoint : public QuicTransportEndpointBase {
 public:
  QuicTransportClientEndpoint(Simulator* simulator,
                              const std::string& name,
                              const std::string& peer_name,
                              url::Origin origin,
                              const std::string& path)
      : QuicTransportEndpointBase(simulator,
                                  name,
                                  peer_name,
                                  Perspective::IS_CLIENT),
        crypto_config_(crypto_test_utils::ProofVerifierForTesting()),
        session_(connection_.get(),
                 nullptr,
                 DefaultQuicConfig(),
                 GetVersions(),
                 GURL("quic-transport://test.example.com:50000" + path),
                 &crypto_config_,
                 origin,
                 &visitor_) {
    session_.Initialize();
  }

  QuicTransportClientSession* session() { return &session_; }
  MockClientVisitor* visitor() { return &visitor_; }

 private:
  QuicCryptoClientConfig crypto_config_;
  MockClientVisitor visitor_;
  QuicTransportClientSession session_;
};

class QuicTransportServerEndpoint : public QuicTransportEndpointBase {
 public:
  QuicTransportServerEndpoint(Simulator* simulator,
                              const std::string& name,
                              const std::string& peer_name,
                              std::vector<url::Origin> accepted_origins)
      : QuicTransportEndpointBase(simulator,
                                  name,
                                  peer_name,
                                  Perspective::IS_SERVER),
        crypto_config_(QuicCryptoServerConfig::TESTING,
                       QuicRandom::GetInstance(),
                       crypto_test_utils::ProofSourceForTesting(),
                       KeyExchangeSource::Default()),
        compressed_certs_cache_(
            QuicCompressedCertsCache::kQuicCompressedCertsCacheSize),
        session_(connection_.get(),
                 /*owns_connection=*/false,
                 nullptr,
                 DefaultQuicConfig(),
                 GetVersions(),
                 &crypto_config_,
                 &compressed_certs_cache_,
                 accepted_origins) {
    session_.Initialize();
  }

  QuicTransportServerSession* session() { return &session_; }

 private:
  QuicCryptoServerConfig crypto_config_;
  QuicCompressedCertsCache compressed_certs_cache_;
  QuicTransportSimpleServerSession session_;
};

std::unique_ptr<MockStreamVisitor> VisitorExpectingFin() {
  auto visitor = std::make_unique<MockStreamVisitor>();
  EXPECT_CALL(*visitor, OnFinRead());
  return visitor;
}

constexpr QuicBandwidth kClientBandwidth =
    QuicBandwidth::FromKBitsPerSecond(10000);
constexpr QuicTime::Delta kClientPropagationDelay =
    QuicTime::Delta::FromMilliseconds(2);
constexpr QuicBandwidth kServerBandwidth =
    QuicBandwidth::FromKBitsPerSecond(4000);
constexpr QuicTime::Delta kServerPropagationDelay =
    QuicTime::Delta::FromMilliseconds(50);
const QuicTime::Delta kTransferTime =
    kClientBandwidth.TransferTime(kMaxOutgoingPacketSize) +
    kServerBandwidth.TransferTime(kMaxOutgoingPacketSize);
const QuicTime::Delta kRtt =
    (kClientPropagationDelay + kServerPropagationDelay + kTransferTime) * 2;
const QuicByteCount kBdp = kRtt * kServerBandwidth;

constexpr QuicTime::Delta kDefaultTimeout = QuicTime::Delta::FromSeconds(3);

class QuicTransportIntegrationTest : public QuicTest {
 public:
  QuicTransportIntegrationTest()
      : switch_(&simulator_, "Switch", 8, 2 * kBdp) {}

  void CreateDefaultEndpoints(const std::string& path) {
    client_ = std::make_unique<QuicTransportClientEndpoint>(
        &simulator_, "Client", "Server", GetTestOrigin(), path);
    server_ = std::make_unique<QuicTransportServerEndpoint>(
        &simulator_, "Server", "Client", accepted_origins_);
  }

  void WireUpEndpoints() {
    client_link_ = std::make_unique<simulator::SymmetricLink>(
        client_.get(), switch_.port(1), kClientBandwidth,
        kClientPropagationDelay);
    server_link_ = std::make_unique<simulator::SymmetricLink>(
        server_.get(), switch_.port(2), kServerBandwidth,
        kServerPropagationDelay);
  }

  void RunHandshake() {
    client_->session()->CryptoConnect();
    bool result = simulator_.RunUntilOrTimeout(
        [this]() {
          return IsHandshakeDone(client_->session()) &&
                 IsHandshakeDone(server_->session());
        },
        kDefaultTimeout);
    EXPECT_TRUE(result);
  }

 protected:
  template <class Session>
  static bool IsHandshakeDone(const Session* session) {
    return session->IsSessionReady() || session->error() != QUIC_NO_ERROR;
  }

  Simulator simulator_;
  simulator::Switch switch_;
  std::unique_ptr<simulator::SymmetricLink> client_link_;
  std::unique_ptr<simulator::SymmetricLink> server_link_;

  std::unique_ptr<QuicTransportClientEndpoint> client_;
  std::unique_ptr<QuicTransportServerEndpoint> server_;

  std::vector<url::Origin> accepted_origins_ = {GetTestOrigin()};
};

TEST_F(QuicTransportIntegrationTest, SuccessfulHandshake) {
  CreateDefaultEndpoints("/discard");
  WireUpEndpoints();
  EXPECT_CALL(*client_->visitor(), OnSessionReady());
  RunHandshake();
  EXPECT_TRUE(client_->session()->IsSessionReady());
  EXPECT_TRUE(server_->session()->IsSessionReady());
}

TEST_F(QuicTransportIntegrationTest, OriginMismatch) {
  accepted_origins_ = {url::Origin::Create(GURL{"https://wrong-origin.test"})};
  CreateDefaultEndpoints("/discard");
  WireUpEndpoints();
  RunHandshake();
  // Wait until the client receives CONNECTION_CLOSE.
  simulator_.RunUntilOrTimeout(
      [this]() { return !client_->session()->connection()->connected(); },
      kDefaultTimeout);
  EXPECT_TRUE(client_->session()->IsSessionReady());
  EXPECT_FALSE(server_->session()->IsSessionReady());
  EXPECT_FALSE(client_->session()->connection()->connected());
  EXPECT_FALSE(server_->session()->connection()->connected());
  EXPECT_THAT(client_->session()->error(),
              IsError(QUIC_TRANSPORT_INVALID_CLIENT_INDICATION));
  EXPECT_THAT(server_->session()->error(),
              IsError(QUIC_TRANSPORT_INVALID_CLIENT_INDICATION));
}

TEST_F(QuicTransportIntegrationTest, SendOutgoingStreams) {
  CreateDefaultEndpoints("/discard");
  WireUpEndpoints();
  RunHandshake();

  std::vector<QuicTransportStream*> streams;
  for (int i = 0; i < 10; i++) {
    QuicTransportStream* stream =
        client_->session()->OpenOutgoingUnidirectionalStream();
    ASSERT_TRUE(stream->Write("test"));
    streams.push_back(stream);
  }
  ASSERT_TRUE(simulator_.RunUntilOrTimeout(
      [this]() {
        return server_->session()->GetNumOpenIncomingStreams() == 10;
      },
      kDefaultTimeout));

  for (QuicTransportStream* stream : streams) {
    ASSERT_TRUE(stream->SendFin());
  }
  ASSERT_TRUE(simulator_.RunUntilOrTimeout(
      [this]() { return server_->session()->GetNumOpenIncomingStreams() == 0; },
      kDefaultTimeout));
}

TEST_F(QuicTransportIntegrationTest, EchoBidirectionalStreams) {
  CreateDefaultEndpoints("/echo");
  WireUpEndpoints();
  RunHandshake();

  QuicTransportStream* stream =
      client_->session()->OpenOutgoingBidirectionalStream();
  EXPECT_TRUE(stream->Write("Hello!"));

  ASSERT_TRUE(simulator_.RunUntilOrTimeout(
      [stream]() { return stream->ReadableBytes() == strlen("Hello!"); },
      kDefaultTimeout));
  std::string received;
  EXPECT_EQ(stream->Read(&received), strlen("Hello!"));
  EXPECT_EQ(received, "Hello!");

  EXPECT_TRUE(stream->SendFin());
  ASSERT_TRUE(simulator_.RunUntilOrTimeout(
      [this]() { return server_->session()->GetNumOpenIncomingStreams() == 0; },
      kDefaultTimeout));
}

TEST_F(QuicTransportIntegrationTest, EchoUnidirectionalStreams) {
  CreateDefaultEndpoints("/echo");
  WireUpEndpoints();
  RunHandshake();

  // Send two streams, but only send FIN on the second one.
  QuicTransportStream* stream1 =
      client_->session()->OpenOutgoingUnidirectionalStream();
  EXPECT_TRUE(stream1->Write("Stream One"));
  QuicTransportStream* stream2 =
      client_->session()->OpenOutgoingUnidirectionalStream();
  EXPECT_TRUE(stream2->Write("Stream Two"));
  EXPECT_TRUE(stream2->SendFin());

  // Wait until a stream is received.
  bool stream_received = false;
  EXPECT_CALL(*client_->visitor(), OnIncomingUnidirectionalStreamAvailable())
      .Times(2)
      .WillRepeatedly(Assign(&stream_received, true));
  ASSERT_TRUE(simulator_.RunUntilOrTimeout(
      [&stream_received]() { return stream_received; }, kDefaultTimeout));

  // Receive a reply stream and expect it to be the second one.
  QuicTransportStream* reply =
      client_->session()->AcceptIncomingUnidirectionalStream();
  ASSERT_TRUE(reply != nullptr);
  std::string buffer;
  reply->set_visitor(VisitorExpectingFin());
  EXPECT_GT(reply->Read(&buffer), 0u);
  EXPECT_EQ(buffer, "Stream Two");

  // Reset reply-related variables.
  stream_received = false;
  buffer = "";

  // Send FIN on the first stream, and expect to receive it back.
  EXPECT_TRUE(stream1->SendFin());
  ASSERT_TRUE(simulator_.RunUntilOrTimeout(
      [&stream_received]() { return stream_received; }, kDefaultTimeout));
  reply = client_->session()->AcceptIncomingUnidirectionalStream();
  ASSERT_TRUE(reply != nullptr);
  reply->set_visitor(VisitorExpectingFin());
  EXPECT_GT(reply->Read(&buffer), 0u);
  EXPECT_EQ(buffer, "Stream One");
}

}  // namespace
}  // namespace test
}  // namespace quic
