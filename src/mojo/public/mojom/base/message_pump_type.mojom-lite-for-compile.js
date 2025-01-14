// mojo/public/mojom/base/message_pump_type.mojom-lite-for-compile.js is auto generated by mojom_bindings_generator.py, do not edit

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
'use strict';

goog.require('mojo.internal');





goog.provide('mojoBase.mojom.MessagePumpType');
goog.provide('mojoBase.mojom.MessagePumpTypeSpec');
/**
 * @const { {$: !mojo.internal.MojomType} }
 * @export
 */
mojoBase.mojom.MessagePumpTypeSpec = { $: mojo.internal.Enum() };

/**
 * @enum {number}
 * @export
 */
mojoBase.mojom.MessagePumpType = {
  
  kDefault: 0,
  kUi: 0,
  kCustom: 0,
  kIo: 0,
  kNsRunloop: 0,
  MIN_VALUE: 0,
  MAX_VALUE: 4,
};

/** @suppress {checkTypes} */
mojoBase.mojom.MessagePumpType.kDefault = 0;

/** @suppress {checkTypes} */
mojoBase.mojom.MessagePumpType.kUi = mojoBase.mojom.MessagePumpType.kDefault + 1;

/** @suppress {checkTypes} */
mojoBase.mojom.MessagePumpType.kCustom = mojoBase.mojom.MessagePumpType.kUi + 1;

/** @suppress {checkTypes} */
mojoBase.mojom.MessagePumpType.kIo = mojoBase.mojom.MessagePumpType.kCustom + 1;

/** @suppress {checkTypes} */
mojoBase.mojom.MessagePumpType.kNsRunloop = mojoBase.mojom.MessagePumpType.kIo + 1;




