project (base)
ADD_DEFINITIONS(-DNOMINMAX -DU_STATIC_IMPLEMENTATION -DU_PLATFORM_HAS_WINUWP_API)
if (WIN32)  
ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN -DUNICODE -D_UNICODE)
endif(WIN32)

set (CMAKE_CXX_STANDARD 14)

add_subdirectory(third_party/boringssl/src boringssl)

if(MSVC)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
endif()

INCLUDE_DIRECTORIES (. .. third_party/boringssl/src/include third_party/icu/source/common)


INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/logging_buildflags.cmake")
INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/sanitizer_buildflags.cmake")
INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/clang_coverage_buildflags.cmake")
INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/allocator_buildflags.cmake")
INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/generated_build_date.cmake")


FILE (GLOB header_files *.h)
FILE (GLOB source_files *.cc)
#移除单元测试
file(GLOB source_to_remove *unittest*)
list(REMOVE_ITEM source_files ${source_to_remove})
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/observer_list_perftest.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/os_compat_nacl.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/pickle_fuzzer.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/rand_util_nacl.cc")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/sync_socket_nacl.cc")

FILE (GLOB memory memory/*.h memory/*.cc)
SOURCE_GROUP("memory" FILES ${memory})
#message(${memory})

FILE (GLOB hash hash/*.h hash/*.cc)
SOURCE_GROUP("hash" FILES ${hash})
list(REMOVE_ITEM hash "${CMAKE_CURRENT_SOURCE_DIR}/hash/sha1_perftest.cc")
#移除单元测试
file(GLOB hash_to_remove hash/*unittest*)
list(REMOVE_ITEM hash ${hash_to_remove})

FILE (GLOB debug debug/*.h debug/*.cc)
SOURCE_GROUP("debug" FILES ${debug})
#生成 debug/debugging_buildflags.h
INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/debugging_buildflags.cmake")
#移除单元测试
file(GLOB debug_to_remove debug/*unittest*)
list(REMOVE_ITEM debug ${debug_to_remove})


#time 目录
FILE (GLOB time time/*.h time/*.cc)
SOURCE_GROUP("time" FILES ${time})
#移除单元测试
file(GLOB time_to_remove time/*unittest*)
list(REMOVE_ITEM time ${time_to_remove})

#metrics 目录
FILE (GLOB metrics metrics/*.h metrics/*.cc)
SOURCE_GROUP("metrics" FILES ${metrics})
#移除单元测试
file(GLOB metrics_to_remove metrics/*unittest*)
list(REMOVE_ITEM metrics ${metrics_to_remove})

#strings 目录
FILE (GLOB strings strings/*.h strings/*.cc)
SOURCE_GROUP("strings" FILES ${strings})
#移除单元测试
file(GLOB strings_to_remove strings/*unittest*)
list(REMOVE_ITEM strings ${strings_to_remove})
list(REMOVE_ITEM strings "${CMAKE_CURRENT_SOURCE_DIR}/strings/string_util_perftest.cc")

FILE (GLOB third_party_cityhash third_party/cityhash/*.h third_party/cityhash/*.cc)
SOURCE_GROUP("third_party\\cityhash" FILES ${third_party_cityhash})

FILE (GLOB third_party_superfasthash third_party/superfasthash/*.h third_party/superfasthash/*.cc third_party/superfasthash/*.c)
SOURCE_GROUP("third_party\\superfasthash" FILES ${third_party_superfasthash})



FILE (GLOB_RECURSE third_party_double_conversion third_party/double_conversion/*.h third_party/double_conversion/*.cc)
SOURCE_GROUP("third_party\\double_conversion" FILES ${third_party_double_conversion})


FILE (GLOB third_party_icu third_party/icu/*.h third_party/icu/*.cc)
SOURCE_GROUP("third_party\\icu" FILES ${third_party_icu})


FILE (GLOB_RECURSE third_party_icu_source_common third_party/icu/source/common/*.h third_party/icu/source/common/*.cpp)
SOURCE_GROUP("third_party\\icu\\source\\common" FILES ${third_party_icu_source_common})

FILE (GLOB_RECURSE third_party_icu_source_i18n third_party/icu/source/i18n/*.h third_party/icu/source/i18n/*.cpp)
SOURCE_GROUP("third_party\\icu\\source\\i18n" FILES ${third_party_icu_source_i18n})



FILE (GLOB threading threading/*.h threading/*.cc)
SOURCE_GROUP("threading" FILES ${threading})
list(REMOVE_ITEM threading "${CMAKE_CURRENT_SOURCE_DIR}/threading/thread_local_storage_perftest.cc")
list(REMOVE_ITEM threading "${CMAKE_CURRENT_SOURCE_DIR}/threading/thread_perftest.cc")
#移除单元测试
file(GLOB threading_to_remove threading/*unittest*)
list(REMOVE_ITEM threading ${threading_to_remove})

FILE (GLOB synchronization synchronization/*.h synchronization/*.cc)
SOURCE_GROUP("synchronization" FILES ${synchronization})
list(REMOVE_ITEM synchronization "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/waitable_event_perftest.cc")
#移除单元测试
file(GLOB synchronization_to_remove synchronization/*unittest*)
list(REMOVE_ITEM synchronization ${synchronization_to_remove})


FILE (GLOB third_party_nspr third_party/nspr/*.h third_party/nspr/*.cc)
SOURCE_GROUP("third_party\\nspr" FILES ${third_party_nspr})

FILE (GLOB containers containers/*.h containers/*.cc)
SOURCE_GROUP("containers" FILES ${containers})
#移除单元测试
file(GLOB containers_to_remove containers/*unittest*)
list(REMOVE_ITEM containers ${containers_to_remove})


FILE (GLOB files files/*.h files/*.cc)
SOURCE_GROUP("files" FILES ${files})
#移除单元测试
file(GLOB files_to_remove files/*unittest*)
list(REMOVE_ITEM files ${files_to_remove})

FILE (GLOB i18n i18n/*.h i18n/*.cc)
SOURCE_GROUP("i18n" FILES ${i18n})
#移除单元测试
file(GLOB i18n_to_remove i18n/*unittest*)
list(REMOVE_ITEM i18n ${i18n_to_remove})
list(REMOVE_ITEM i18n "${CMAKE_CURRENT_SOURCE_DIR}/i18n/streaming_utf8_validator_perftest.cc")

FILE (GLOB timer timer/*.h timer/*.cc)
SOURCE_GROUP("timer" FILES ${timer})
#移除单元测试
file(GLOB timer_to_remove timer/*unittest*)
list(REMOVE_ITEM timer ${timer_to_remove})

FILE (GLOB_RECURSE util util/*.h util/*.cc)
SOURCE_GROUP("util" FILES ${util})
#移除单元测试
file(GLOB_RECURSE util_to_remove util/*unittest*)
list(REMOVE_ITEM util ${util_to_remove})

FILE (GLOB_RECURSE trace_event trace_event/*.h trace_event/*.cc)
SOURCE_GROUP("trace_event" FILES ${trace_event})
#移除单元测试
file(GLOB_RECURSE trace_event_to_remove trace_event/*unittest*)
list(REMOVE_ITEM trace_event ${trace_event_to_remove})
list(REMOVE_ITEM trace_event "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/trace_event_filter_test_utils.cc")
list(REMOVE_ITEM trace_event "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/trace_event_filter_test_utils.h")
list(REMOVE_ITEM trace_event "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/trace_event_etw_export_win.cc")
list(REMOVE_ITEM trace_event "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/trace_event_etw_export_win.h")



FILE (GLOB_RECURSE system system/*.h system/*.cc)
SOURCE_GROUP("system" FILES ${system})
#移除单元测试
file(GLOB_RECURSE system_to_remove system/*unittest*)
list(REMOVE_ITEM system ${system_to_remove})

FILE (GLOB_RECURSE allocator allocator/*.h allocator/*.cc)
SOURCE_GROUP("allocator" FILES ${allocator})
#移除单元测试
file(GLOB_RECURSE allocator_to_remove allocator/*unittest*)
list(REMOVE_ITEM allocator ${allocator_to_remove})
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/partition_allocator/spin_lock_perftest.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/partition_allocator/partition_alloc_perftest.cc")
#不适用 shim
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_internals.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_override_cpp_symbols.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_override_cpp_symbols.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_default_dispatch_to_tcmalloc.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_override_glibc_weak_symbols.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_default_dispatch_to_mac_zoned_malloc.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_default_dispatch_to_mac_zoned_malloc.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_default_dispatch_to_linker_wrapped_symbols.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_override_mac_symbols.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_override_linker_wrapped_symbols.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_default_dispatch_to_winheap.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_default_dispatch_to_glibc.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_shim_override_ucrt_symbols_win.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/winheap_stubs_win.cc")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/winheap_stubs_win.h")
list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/debugallocation_shim.cc")


#profiler
FILE (GLOB_RECURSE profiler profiler/*.h profiler/*.cc)
SOURCE_GROUP("profiler" FILES ${profiler})
#移除单元测试
file(GLOB_RECURSE profiler_to_remove profiler/*unittest*)
list(REMOVE_ITEM profiler ${profiler_to_remove})
list(REMOVE_ITEM profiler "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_sampling_profiler_test_util.h")
list(REMOVE_ITEM profiler "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_sampling_profiler_test_util.cc")


#process
FILE (GLOB_RECURSE process process/*.h process/*.cc)
SOURCE_GROUP("process" FILES ${process})
#移除单元测试
file(GLOB_RECURSE process_to_remove process/*unittest*)
list(REMOVE_ITEM process ${process_to_remove})


#power_monitor
FILE (GLOB_RECURSE power_monitor power_monitor/*.h power_monitor/*.cc)
SOURCE_GROUP("power_monitor" FILES ${power_monitor})
#移除单元测试
file(GLOB_RECURSE power_monitor_to_remove power_monitor/*unittest*)
list(REMOVE_ITEM power_monitor ${power_monitor_to_remove})
list(REMOVE_ITEM power_monitor "${CMAKE_CURRENT_SOURCE_DIR}/power_monitor/power_monitor_device_source_stub.cc")


#numerics
FILE (GLOB_RECURSE numerics numerics/*.h numerics/*.cc)
SOURCE_GROUP("numerics" FILES ${numerics})



#json
FILE (GLOB_RECURSE json json/*.h json/*.cc)
SOURCE_GROUP("json" FILES ${json})
#移除单元测试
file(GLOB_RECURSE json_to_remove json/*unittest*)
list(REMOVE_ITEM json ${json_to_remove})
list(REMOVE_ITEM json "${CMAKE_CURRENT_SOURCE_DIR}/json/json_perftest.cc")


#message_loop
FILE (GLOB_RECURSE message_loop message_loop/*.h message_loop/*.cc)
SOURCE_GROUP("message_loop" FILES ${message_loop})
#移除单元测试
file(GLOB_RECURSE message_loop_to_remove message_loop/*unittest*)
list(REMOVE_ITEM message_loop ${message_loop_to_remove})
list(REMOVE_ITEM message_loop "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_perftest.cc")


#memory
FILE (GLOB_RECURSE memory memory/*.h memory/*.cc)
SOURCE_GROUP("memory" FILES ${memory})
#移除单元测试
file(GLOB_RECURSE memory_to_remove memory/*unittest*)
list(REMOVE_ITEM memory ${memory_to_remove})

#task
FILE (GLOB_RECURSE task task/*.h task/*.cc)
SOURCE_GROUP("task" FILES ${task})
#移除单元测试
file(GLOB_RECURSE task_to_remove task/*unittest*)
list(REMOVE_ITEM task ${task_to_remove})
#移除test目录
file(GLOB task_test_to_remove task/sequence_manager/test/*.*)
list(REMOVE_ITEM task ${task_test_to_remove})

list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/sequence_manager/sequence_manager_perftest.cc")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/thread_pool/thread_pool_perftest.cc")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/thread_pool/test_utils.cc")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/thread_pool/test_utils.h")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/thread_pool/test_task_factory.cc")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/thread_pool/test_task_factory.h")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/sequence_manager/test_task_queue.h")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/sequence_manager/test_task_queue.cc")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/test_task_traits_extension.h")
list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/test_task_traits_extension.cc")


if (WIN32)  
    list(REMOVE_ITEM memory "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_allocator_posix.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_allocator_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_mac.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_fuchsia.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_posix.cc")

	  list(REMOVE_ITEM threading "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_android.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_fuchsia.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_internal_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_internal_posix.h"
	 "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_linux.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/threading/thread_local_storage_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/threading/platform_thread_mac.mm")
    
	 list(REMOVE_ITEM debug "${CMAKE_CURRENT_SOURCE_DIR}/debug/debugger_posix.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/debug/proc_maps_linux.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/proc_maps_linux.h"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/stack_trace_android.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/stack_trace_fuchsia.h"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/stack_trace_fuchsia.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/elf_reader.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/elf_reader.h"
	 "${CMAKE_CURRENT_SOURCE_DIR}/debug/stack_trace_posix.cc")


	  list(REMOVE_ITEM synchronization "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/condition_variable_posix.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/lock_impl_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/waitable_event_mac.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/waitable_event_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/waitable_event_watcher_mac.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/waitable_event_watcher_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/thread_local_storage_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/synchronization/platform_thread_mac.mm")


	 list(REMOVE_ITEM time "${CMAKE_CURRENT_SOURCE_DIR}/time/time_android.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/time/time_conversion_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/time/time_exploded_ios.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/time/time_exploded_posix.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/time/time_fuchsia.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/time/time_mac.cc"
	 "${CMAKE_CURRENT_SOURCE_DIR}/time/time_now_posix.cc")

	list(REMOVE_ITEM files "${CMAKE_CURRENT_SOURCE_DIR}/files/file_descriptor_watcher_posix.cc"
		  "${CMAKE_CURRENT_SOURCE_DIR}/files/file_descriptor_watcher_posix.h"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_enumerator_posix.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_linux.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_mac.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_posix.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_posix.h"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_util_android.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_util_linux.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_util_posix.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/memory_mapped_file_posix.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/scoped_file_android.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_kqueue.h"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_kqueue.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_fuchsia.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_fsevents.cc"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/file_path_watcher_fsevents.h"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/dir_reader_posix.h"
		 "${CMAKE_CURRENT_SOURCE_DIR}/files/dir_reader_linux.h")

	list(REMOVE_ITEM strings "${CMAKE_CURRENT_SOURCE_DIR}/strings/string_util_posix.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/strings/sys_string_conversions_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/strings/file_descriptor_store.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/strings/file_descriptor_store.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/strings/string16.cc")

	list(REMOVE_ITEM util "${CMAKE_CURRENT_SOURCE_DIR}/util/memory_pressure/system_memory_pressure_evaluator_chromeos.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/util/memory_pressure/system_memory_pressure_evaluator_chromeos.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/util/memory_pressure/system_memory_pressure_evaluator_mac.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/util/memory_pressure/system_memory_pressure_evaluator_mac.h")


	list(REMOVE_ITEM trace_event "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/cpufreq_monitor_android.h"
      "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/cpufreq_monitor_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/cfi_backtrace_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/cfi_backtrace_android.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/java_heap_dump_provider_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/java_heap_dump_provider_android.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/trace_event/trace_event_android.cc"	)

	 list(REMOVE_ITEM system "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_android.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_chromeos.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_freebsd.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_fuchsia.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_linux.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_openbsd.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/system/sys_info_posix.cc")

	  list(REMOVE_ITEM profiler "${CMAKE_CURRENT_SOURCE_DIR}/profiler/native_unwinder_android.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/profiler/native_unwinder_android.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/native_unwinder_mac.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/native_unwinder_mac.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_sampler_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/thread_delegate_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/thread_delegate_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/suspendable_thread_delegate_mac.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/suspendable_thread_delegate_mac.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_sampler_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_copier_signal.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_sampler_ios.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/profiler/stack_sampler_mac.cc")

	  list(REMOVE_ITEM allocator "${CMAKE_CURRENT_SOURCE_DIR}/allocator/partition_allocator/page_allocator_internals_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/allocator/partition_allocator/page_allocator_internals_fuchsia.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/allocator/malloc_zone_functions_mac.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/allocator/malloc_zone_functions_mac.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/allocator/allocator_interception_mac.h")

	  list(REMOVE_ITEM power_monitor "${CMAKE_CURRENT_SOURCE_DIR}/power_monitor/power_monitor_device_source_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/power_monitor/power_monitor_device_source_chromeos.cc")

	 list(REMOVE_ITEM message_loop "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/watchable_io_message_pump_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/watchable_io_message_pump_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_libevent.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_libevent.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_fuchsia.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_fuchsia.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_android.h"
	   "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_io_ios.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_io_ios.h"
	    "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_glib.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_glib.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_kqueue.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_kqueue.h"	  
	  "${CMAKE_CURRENT_SOURCE_DIR}/message_loop/message_pump_mac.h")

	   list(REMOVE_ITEM memory "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_allocator_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_allocator_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_posix.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/madv_free_discardable_memory_posix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_android.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_android.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_mac.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/memory/platform_shared_memory_region_posix.cc")
	  

	  #process 移除其他平台
	  file(GLOB_RECURSE process_posix_to_remove process/*_posix.*)
	  list(REMOVE_ITEM process ${process_posix_to_remove})
	   file(GLOB_RECURSE process_mac_to_remove process/*_mac.*)
	  list(REMOVE_ITEM process ${process_mac_to_remove})
	  file(GLOB_RECURSE process_ios_to_remove process/*_ios.*)
	  list(REMOVE_ITEM process ${process_ios_to_remove})
	   file(GLOB_RECURSE process_linux_to_remove process/*_linux.*)
	  list(REMOVE_ITEM process ${process_linux_to_remove})
	   file(GLOB_RECURSE process_openbsd_to_remove process/*_openbsd.*)
	  list(REMOVE_ITEM process ${process_openbsd_to_remove})
	  file(GLOB_RECURSE process_freebsd_to_remove process/*_freebsd.*)
	  list(REMOVE_ITEM process ${process_freebsd_to_remove})
	  file(GLOB_RECURSE process_fuchsia_to_remove process/*_fuchsia.*)
	  list(REMOVE_ITEM process ${process_fuchsia_to_remove})
	    list(REMOVE_ITEM process "${CMAKE_CURRENT_SOURCE_DIR}/process/internal_aix.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/process/internal_aix.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/process/process_metrics_nacl.cc")



	   #source_files 移除其他平台
	  file(GLOB_RECURSE source_posix_to_remove *_posix.*)
	  list(REMOVE_ITEM source_files ${source_posix_to_remove})
	   file(GLOB_RECURSE source_mac_to_remove *_mac.*)
	  list(REMOVE_ITEM source_files ${source_mac_to_remove})
	  file(GLOB_RECURSE source_ios_to_remove *_ios.*)
	  list(REMOVE_ITEM source_files ${source_ios_to_remove})
	   file(GLOB_RECURSE source_linux_to_remove *_linux.*)
	  list(REMOVE_ITEM source_files ${source_linux_to_remove})
	   file(GLOB_RECURSE source_openbsd_to_remove *_openbsd.*)
	  list(REMOVE_ITEM source_files ${source_openbsd_to_remove})
	  file(GLOB_RECURSE source_freebsd_to_remove *_freebsd.*)
	  list(REMOVE_ITEM source_files ${source_freebsd_to_remove})
	  file(GLOB_RECURSE source_fuchsia_to_remove *_fuchsia.*)
	  list(REMOVE_ITEM source_files ${source_fuchsia_to_remove})
	  file(GLOB_RECURSE source_android_to_remove *_android.*)
	  list(REMOVE_ITEM source_files ${source_android_to_remove})
	   list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/file_descriptor_store.cc"
      "${CMAKE_CURRENT_SOURCE_DIR}/file_descriptor_store.h"
	  "${CMAKE_CURRENT_SOURCE_DIR}/linux_util.cc")


	   list(REMOVE_ITEM task "${CMAKE_CURRENT_SOURCE_DIR}/task/thread_pool/task_tracker_posix.cc")


	 list(REMOVE_ITEM timer "${CMAKE_CURRENT_SOURCE_DIR}/timer/hi_res_timer_manager_posix.cc")

	 FILE (GLOB_RECURSE win win/*.h win/*.cc)
	 SOURCE_GROUP("win" FILES ${win})

	#移除单元测试
	file(GLOB_RECURSE win_to_remove win/*unittest*)
	list(REMOVE_ITEM win ${win_to_remove})
	INCLUDE ("${CMAKE_CURRENT_SOURCE_DIR}/auto_gen/base_win_buildflags.cmake")

elseif (IOS)


elseif (OSX)


elseif (ANDROID)

endif(WIN32)
 
 
ADD_LIBRARY (base ${task} ${third_party_superfasthash} ${third_party_icu_source_i18n} ${third_party_icu_source_common}  ${header_files} ${source_files} ${memory} ${message_loop} ${json} ${numerics} ${power_monitor} ${process} ${profiler} ${allocator} ${system} ${trace_event} ${util} ${timer} ${hash} ${strings} ${files} ${containers} ${synchronization}  ${third_party_icu} ${threading} ${win} ${debug}  ${time} ${metrics} ${third_party_nspr} ${third_party_double_conversion} ${third_party_cityhash})
