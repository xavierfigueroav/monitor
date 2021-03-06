/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: monitor.proto */

#ifndef PROTOBUF_C_monitor_2eproto__INCLUDED
#define PROTOBUF_C_monitor_2eproto__INCLUDED

#include "../include/protobuf-c.h"

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _SystemInfo SystemInfo;
typedef struct _PerformanceInfo PerformanceInfo;
typedef struct _PerformanceInfo__LoadAvg PerformanceInfo__LoadAvg;


/* --- enums --- */


/* --- messages --- */

struct  _SystemInfo
{
  ProtobufCMessage base;
  char *system_name;
  char *version;
  protobuf_c_boolean has_num_processors;
  int32_t num_processors;
  /*
   *en KB
   */
  protobuf_c_boolean has_mem_total;
  int32_t mem_total;
};
#define SYSTEM_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&system_info__descriptor) \
    , NULL, NULL, 0,0, 0,0 }


struct  _PerformanceInfo__LoadAvg
{
  ProtobufCMessage base;
  protobuf_c_boolean has_min5;
  float min5;
  protobuf_c_boolean has_min10;
  float min10;
  protobuf_c_boolean has_min15;
  float min15;
};
#define PERFORMANCE_INFO__LOAD_AVG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&performance_info__load_avg__descriptor) \
    , 0,0, 0,0, 0,0 }


struct  _PerformanceInfo
{
  ProtobufCMessage base;
  protobuf_c_boolean has_uptime;
  float uptime;
  /*
   *en porcentaje, un valor por procesador
   */
  size_t n_processor_usage;
  int32_t *processor_usage;
  /*
   *en KB
   */
  protobuf_c_boolean has_mem_free;
  int32_t mem_free;
  protobuf_c_boolean has_num_process;
  int32_t num_process;
  protobuf_c_boolean has_num_process_running;
  int32_t num_process_running;
  PerformanceInfo__LoadAvg *loadavg;
};
#define PERFORMANCE_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&performance_info__descriptor) \
    , 0,0, 0,NULL, 0,0, 0,0, 0,0, NULL }


/* SystemInfo methods */
void   system_info__init
                     (SystemInfo         *message);
size_t system_info__get_packed_size
                     (const SystemInfo   *message);
size_t system_info__pack
                     (const SystemInfo   *message,
                      uint8_t             *out);
size_t system_info__pack_to_buffer
                     (const SystemInfo   *message,
                      ProtobufCBuffer     *buffer);
SystemInfo *
       system_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   system_info__free_unpacked
                     (SystemInfo *message,
                      ProtobufCAllocator *allocator);
/* PerformanceInfo__LoadAvg methods */
void   performance_info__load_avg__init
                     (PerformanceInfo__LoadAvg         *message);
/* PerformanceInfo methods */
void   performance_info__init
                     (PerformanceInfo         *message);
size_t performance_info__get_packed_size
                     (const PerformanceInfo   *message);
size_t performance_info__pack
                     (const PerformanceInfo   *message,
                      uint8_t             *out);
size_t performance_info__pack_to_buffer
                     (const PerformanceInfo   *message,
                      ProtobufCBuffer     *buffer);
PerformanceInfo *
       performance_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   performance_info__free_unpacked
                     (PerformanceInfo *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SystemInfo_Closure)
                 (const SystemInfo *message,
                  void *closure_data);
typedef void (*PerformanceInfo__LoadAvg_Closure)
                 (const PerformanceInfo__LoadAvg *message,
                  void *closure_data);
typedef void (*PerformanceInfo_Closure)
                 (const PerformanceInfo *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor system_info__descriptor;
extern const ProtobufCMessageDescriptor performance_info__descriptor;
extern const ProtobufCMessageDescriptor performance_info__load_avg__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_monitor_2eproto__INCLUDED */
