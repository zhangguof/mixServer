// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: test.proto

#ifndef PROTOBUF_test_2eproto__INCLUDED
#define PROTOBUF_test_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_test_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[4];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
void InitDefaultsAddRetImpl();
void InitDefaultsAddRet();
void InitDefaultsEchoImpl();
void InitDefaultsEcho();
void InitDefaultsC_EchoImpl();
void InitDefaultsC_Echo();
void InitDefaultsAddImpl();
void InitDefaultsAdd();
inline void InitDefaults() {
  InitDefaultsAddRet();
  InitDefaultsEcho();
  InitDefaultsC_Echo();
  InitDefaultsAdd();
}
}  // namespace protobuf_test_2eproto
namespace proto {
class Add;
class AddDefaultTypeInternal;
extern AddDefaultTypeInternal _Add_default_instance_;
class AddRet;
class AddRetDefaultTypeInternal;
extern AddRetDefaultTypeInternal _AddRet_default_instance_;
class C_Echo;
class C_EchoDefaultTypeInternal;
extern C_EchoDefaultTypeInternal _C_Echo_default_instance_;
class Echo;
class EchoDefaultTypeInternal;
extern EchoDefaultTypeInternal _Echo_default_instance_;
}  // namespace proto
namespace proto {

// ===================================================================

class AddRet : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.AddRet) */ {
 public:
  AddRet();
  virtual ~AddRet();

  AddRet(const AddRet& from);

  inline AddRet& operator=(const AddRet& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AddRet(AddRet&& from) noexcept
    : AddRet() {
    *this = ::std::move(from);
  }

  inline AddRet& operator=(AddRet&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AddRet& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AddRet* internal_default_instance() {
    return reinterpret_cast<const AddRet*>(
               &_AddRet_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(AddRet* other);
  friend void swap(AddRet& a, AddRet& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AddRet* New() const PROTOBUF_FINAL { return New(NULL); }

  AddRet* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const AddRet& from);
  void MergeFrom(const AddRet& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(AddRet* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 ret = 3;
  bool has_ret() const;
  void clear_ret();
  static const int kRetFieldNumber = 3;
  ::google::protobuf::int32 ret() const;
  void set_ret(::google::protobuf::int32 value);

  // optional int32 service_id = 1 [default = 1000];
  bool has_service_id() const;
  void clear_service_id();
  static const int kServiceIdFieldNumber = 1;
  ::google::protobuf::int32 service_id() const;
  void set_service_id(::google::protobuf::int32 value);

  // optional int32 command_id = 2 [default = 1002];
  bool has_command_id() const;
  void clear_command_id();
  static const int kCommandIdFieldNumber = 2;
  ::google::protobuf::int32 command_id() const;
  void set_command_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.AddRet)
 private:
  void set_has_service_id();
  void clear_has_service_id();
  void set_has_command_id();
  void clear_has_command_id();
  void set_has_ret();
  void clear_has_ret();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::int32 ret_;
  ::google::protobuf::int32 service_id_;
  ::google::protobuf::int32 command_id_;
  friend struct ::protobuf_test_2eproto::TableStruct;
  friend void ::protobuf_test_2eproto::InitDefaultsAddRetImpl();
};
// -------------------------------------------------------------------

class Echo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.Echo) */ {
 public:
  Echo();
  virtual ~Echo();

  Echo(const Echo& from);

  inline Echo& operator=(const Echo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Echo(Echo&& from) noexcept
    : Echo() {
    *this = ::std::move(from);
  }

  inline Echo& operator=(Echo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Echo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Echo* internal_default_instance() {
    return reinterpret_cast<const Echo*>(
               &_Echo_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(Echo* other);
  friend void swap(Echo& a, Echo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Echo* New() const PROTOBUF_FINAL { return New(NULL); }

  Echo* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Echo& from);
  void MergeFrom(const Echo& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Echo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string msg = 3;
  bool has_msg() const;
  void clear_msg();
  static const int kMsgFieldNumber = 3;
  const ::std::string& msg() const;
  void set_msg(const ::std::string& value);
  #if LANG_CXX11
  void set_msg(::std::string&& value);
  #endif
  void set_msg(const char* value);
  void set_msg(const char* value, size_t size);
  ::std::string* mutable_msg();
  ::std::string* release_msg();
  void set_allocated_msg(::std::string* msg);

  // optional int32 service_id = 1 [default = 1000];
  bool has_service_id() const;
  void clear_service_id();
  static const int kServiceIdFieldNumber = 1;
  ::google::protobuf::int32 service_id() const;
  void set_service_id(::google::protobuf::int32 value);

  // optional int32 command_id = 2 [default = 1];
  bool has_command_id() const;
  void clear_command_id();
  static const int kCommandIdFieldNumber = 2;
  ::google::protobuf::int32 command_id() const;
  void set_command_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.Echo)
 private:
  void set_has_service_id();
  void clear_has_service_id();
  void set_has_command_id();
  void clear_has_command_id();
  void set_has_msg();
  void clear_has_msg();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr msg_;
  ::google::protobuf::int32 service_id_;
  ::google::protobuf::int32 command_id_;
  friend struct ::protobuf_test_2eproto::TableStruct;
  friend void ::protobuf_test_2eproto::InitDefaultsEchoImpl();
};
// -------------------------------------------------------------------

class C_Echo : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.C_Echo) */ {
 public:
  C_Echo();
  virtual ~C_Echo();

  C_Echo(const C_Echo& from);

  inline C_Echo& operator=(const C_Echo& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  C_Echo(C_Echo&& from) noexcept
    : C_Echo() {
    *this = ::std::move(from);
  }

  inline C_Echo& operator=(C_Echo&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const C_Echo& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const C_Echo* internal_default_instance() {
    return reinterpret_cast<const C_Echo*>(
               &_C_Echo_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    2;

  void Swap(C_Echo* other);
  friend void swap(C_Echo& a, C_Echo& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline C_Echo* New() const PROTOBUF_FINAL { return New(NULL); }

  C_Echo* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const C_Echo& from);
  void MergeFrom(const C_Echo& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(C_Echo* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string msg = 3;
  bool has_msg() const;
  void clear_msg();
  static const int kMsgFieldNumber = 3;
  const ::std::string& msg() const;
  void set_msg(const ::std::string& value);
  #if LANG_CXX11
  void set_msg(::std::string&& value);
  #endif
  void set_msg(const char* value);
  void set_msg(const char* value, size_t size);
  ::std::string* mutable_msg();
  ::std::string* release_msg();
  void set_allocated_msg(::std::string* msg);

  // optional int32 service_id = 1 [default = 1000];
  bool has_service_id() const;
  void clear_service_id();
  static const int kServiceIdFieldNumber = 1;
  ::google::protobuf::int32 service_id() const;
  void set_service_id(::google::protobuf::int32 value);

  // optional int32 command_id = 2 [default = 2];
  bool has_command_id() const;
  void clear_command_id();
  static const int kCommandIdFieldNumber = 2;
  ::google::protobuf::int32 command_id() const;
  void set_command_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.C_Echo)
 private:
  void set_has_service_id();
  void clear_has_service_id();
  void set_has_command_id();
  void clear_has_command_id();
  void set_has_msg();
  void clear_has_msg();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr msg_;
  ::google::protobuf::int32 service_id_;
  ::google::protobuf::int32 command_id_;
  friend struct ::protobuf_test_2eproto::TableStruct;
  friend void ::protobuf_test_2eproto::InitDefaultsC_EchoImpl();
};
// -------------------------------------------------------------------

class Add : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto.Add) */ {
 public:
  Add();
  virtual ~Add();

  Add(const Add& from);

  inline Add& operator=(const Add& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Add(Add&& from) noexcept
    : Add() {
    *this = ::std::move(from);
  }

  inline Add& operator=(Add&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Add& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Add* internal_default_instance() {
    return reinterpret_cast<const Add*>(
               &_Add_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    3;

  void Swap(Add* other);
  friend void swap(Add& a, Add& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Add* New() const PROTOBUF_FINAL { return New(NULL); }

  Add* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Add& from);
  void MergeFrom(const Add& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Add* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 add1 = 3;
  bool has_add1() const;
  void clear_add1();
  static const int kAdd1FieldNumber = 3;
  ::google::protobuf::int32 add1() const;
  void set_add1(::google::protobuf::int32 value);

  // required int32 add2 = 4;
  bool has_add2() const;
  void clear_add2();
  static const int kAdd2FieldNumber = 4;
  ::google::protobuf::int32 add2() const;
  void set_add2(::google::protobuf::int32 value);

  // optional int32 service_id = 1 [default = 1000];
  bool has_service_id() const;
  void clear_service_id();
  static const int kServiceIdFieldNumber = 1;
  ::google::protobuf::int32 service_id() const;
  void set_service_id(::google::protobuf::int32 value);

  // optional int32 command_id = 2 [default = 1001];
  bool has_command_id() const;
  void clear_command_id();
  static const int kCommandIdFieldNumber = 2;
  ::google::protobuf::int32 command_id() const;
  void set_command_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.Add)
 private:
  void set_has_service_id();
  void clear_has_service_id();
  void set_has_command_id();
  void clear_has_command_id();
  void set_has_add1();
  void clear_has_add1();
  void set_has_add2();
  void clear_has_add2();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::int32 add1_;
  ::google::protobuf::int32 add2_;
  ::google::protobuf::int32 service_id_;
  ::google::protobuf::int32 command_id_;
  friend struct ::protobuf_test_2eproto::TableStruct;
  friend void ::protobuf_test_2eproto::InitDefaultsAddImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AddRet

// optional int32 service_id = 1 [default = 1000];
inline bool AddRet::has_service_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AddRet::set_has_service_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AddRet::clear_has_service_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AddRet::clear_service_id() {
  service_id_ = 1000;
  clear_has_service_id();
}
inline ::google::protobuf::int32 AddRet::service_id() const {
  // @@protoc_insertion_point(field_get:proto.AddRet.service_id)
  return service_id_;
}
inline void AddRet::set_service_id(::google::protobuf::int32 value) {
  set_has_service_id();
  service_id_ = value;
  // @@protoc_insertion_point(field_set:proto.AddRet.service_id)
}

// optional int32 command_id = 2 [default = 1002];
inline bool AddRet::has_command_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void AddRet::set_has_command_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void AddRet::clear_has_command_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void AddRet::clear_command_id() {
  command_id_ = 1002;
  clear_has_command_id();
}
inline ::google::protobuf::int32 AddRet::command_id() const {
  // @@protoc_insertion_point(field_get:proto.AddRet.command_id)
  return command_id_;
}
inline void AddRet::set_command_id(::google::protobuf::int32 value) {
  set_has_command_id();
  command_id_ = value;
  // @@protoc_insertion_point(field_set:proto.AddRet.command_id)
}

// required int32 ret = 3;
inline bool AddRet::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AddRet::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AddRet::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AddRet::clear_ret() {
  ret_ = 0;
  clear_has_ret();
}
inline ::google::protobuf::int32 AddRet::ret() const {
  // @@protoc_insertion_point(field_get:proto.AddRet.ret)
  return ret_;
}
inline void AddRet::set_ret(::google::protobuf::int32 value) {
  set_has_ret();
  ret_ = value;
  // @@protoc_insertion_point(field_set:proto.AddRet.ret)
}

// -------------------------------------------------------------------

// Echo

// optional int32 service_id = 1 [default = 1000];
inline bool Echo::has_service_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Echo::set_has_service_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Echo::clear_has_service_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Echo::clear_service_id() {
  service_id_ = 1000;
  clear_has_service_id();
}
inline ::google::protobuf::int32 Echo::service_id() const {
  // @@protoc_insertion_point(field_get:proto.Echo.service_id)
  return service_id_;
}
inline void Echo::set_service_id(::google::protobuf::int32 value) {
  set_has_service_id();
  service_id_ = value;
  // @@protoc_insertion_point(field_set:proto.Echo.service_id)
}

// optional int32 command_id = 2 [default = 1];
inline bool Echo::has_command_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Echo::set_has_command_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Echo::clear_has_command_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Echo::clear_command_id() {
  command_id_ = 1;
  clear_has_command_id();
}
inline ::google::protobuf::int32 Echo::command_id() const {
  // @@protoc_insertion_point(field_get:proto.Echo.command_id)
  return command_id_;
}
inline void Echo::set_command_id(::google::protobuf::int32 value) {
  set_has_command_id();
  command_id_ = value;
  // @@protoc_insertion_point(field_set:proto.Echo.command_id)
}

// required string msg = 3;
inline bool Echo::has_msg() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Echo::set_has_msg() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Echo::clear_has_msg() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Echo::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_msg();
}
inline const ::std::string& Echo::msg() const {
  // @@protoc_insertion_point(field_get:proto.Echo.msg)
  return msg_.GetNoArena();
}
inline void Echo::set_msg(const ::std::string& value) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto.Echo.msg)
}
#if LANG_CXX11
inline void Echo::set_msg(::std::string&& value) {
  set_has_msg();
  msg_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto.Echo.msg)
}
#endif
inline void Echo::set_msg(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto.Echo.msg)
}
inline void Echo::set_msg(const char* value, size_t size) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto.Echo.msg)
}
inline ::std::string* Echo::mutable_msg() {
  set_has_msg();
  // @@protoc_insertion_point(field_mutable:proto.Echo.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Echo::release_msg() {
  // @@protoc_insertion_point(field_release:proto.Echo.msg)
  clear_has_msg();
  return msg_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Echo::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    set_has_msg();
  } else {
    clear_has_msg();
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:proto.Echo.msg)
}

// -------------------------------------------------------------------

// C_Echo

// optional int32 service_id = 1 [default = 1000];
inline bool C_Echo::has_service_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void C_Echo::set_has_service_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void C_Echo::clear_has_service_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void C_Echo::clear_service_id() {
  service_id_ = 1000;
  clear_has_service_id();
}
inline ::google::protobuf::int32 C_Echo::service_id() const {
  // @@protoc_insertion_point(field_get:proto.C_Echo.service_id)
  return service_id_;
}
inline void C_Echo::set_service_id(::google::protobuf::int32 value) {
  set_has_service_id();
  service_id_ = value;
  // @@protoc_insertion_point(field_set:proto.C_Echo.service_id)
}

// optional int32 command_id = 2 [default = 2];
inline bool C_Echo::has_command_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void C_Echo::set_has_command_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void C_Echo::clear_has_command_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void C_Echo::clear_command_id() {
  command_id_ = 2;
  clear_has_command_id();
}
inline ::google::protobuf::int32 C_Echo::command_id() const {
  // @@protoc_insertion_point(field_get:proto.C_Echo.command_id)
  return command_id_;
}
inline void C_Echo::set_command_id(::google::protobuf::int32 value) {
  set_has_command_id();
  command_id_ = value;
  // @@protoc_insertion_point(field_set:proto.C_Echo.command_id)
}

// required string msg = 3;
inline bool C_Echo::has_msg() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void C_Echo::set_has_msg() {
  _has_bits_[0] |= 0x00000001u;
}
inline void C_Echo::clear_has_msg() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void C_Echo::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_msg();
}
inline const ::std::string& C_Echo::msg() const {
  // @@protoc_insertion_point(field_get:proto.C_Echo.msg)
  return msg_.GetNoArena();
}
inline void C_Echo::set_msg(const ::std::string& value) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto.C_Echo.msg)
}
#if LANG_CXX11
inline void C_Echo::set_msg(::std::string&& value) {
  set_has_msg();
  msg_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto.C_Echo.msg)
}
#endif
inline void C_Echo::set_msg(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto.C_Echo.msg)
}
inline void C_Echo::set_msg(const char* value, size_t size) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto.C_Echo.msg)
}
inline ::std::string* C_Echo::mutable_msg() {
  set_has_msg();
  // @@protoc_insertion_point(field_mutable:proto.C_Echo.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* C_Echo::release_msg() {
  // @@protoc_insertion_point(field_release:proto.C_Echo.msg)
  clear_has_msg();
  return msg_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void C_Echo::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    set_has_msg();
  } else {
    clear_has_msg();
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:proto.C_Echo.msg)
}

// -------------------------------------------------------------------

// Add

// optional int32 service_id = 1 [default = 1000];
inline bool Add::has_service_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Add::set_has_service_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Add::clear_has_service_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Add::clear_service_id() {
  service_id_ = 1000;
  clear_has_service_id();
}
inline ::google::protobuf::int32 Add::service_id() const {
  // @@protoc_insertion_point(field_get:proto.Add.service_id)
  return service_id_;
}
inline void Add::set_service_id(::google::protobuf::int32 value) {
  set_has_service_id();
  service_id_ = value;
  // @@protoc_insertion_point(field_set:proto.Add.service_id)
}

// optional int32 command_id = 2 [default = 1001];
inline bool Add::has_command_id() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Add::set_has_command_id() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Add::clear_has_command_id() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Add::clear_command_id() {
  command_id_ = 1001;
  clear_has_command_id();
}
inline ::google::protobuf::int32 Add::command_id() const {
  // @@protoc_insertion_point(field_get:proto.Add.command_id)
  return command_id_;
}
inline void Add::set_command_id(::google::protobuf::int32 value) {
  set_has_command_id();
  command_id_ = value;
  // @@protoc_insertion_point(field_set:proto.Add.command_id)
}

// required int32 add1 = 3;
inline bool Add::has_add1() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Add::set_has_add1() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Add::clear_has_add1() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Add::clear_add1() {
  add1_ = 0;
  clear_has_add1();
}
inline ::google::protobuf::int32 Add::add1() const {
  // @@protoc_insertion_point(field_get:proto.Add.add1)
  return add1_;
}
inline void Add::set_add1(::google::protobuf::int32 value) {
  set_has_add1();
  add1_ = value;
  // @@protoc_insertion_point(field_set:proto.Add.add1)
}

// required int32 add2 = 4;
inline bool Add::has_add2() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Add::set_has_add2() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Add::clear_has_add2() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Add::clear_add2() {
  add2_ = 0;
  clear_has_add2();
}
inline ::google::protobuf::int32 Add::add2() const {
  // @@protoc_insertion_point(field_get:proto.Add.add2)
  return add2_;
}
inline void Add::set_add2(::google::protobuf::int32 value) {
  set_has_add2();
  add2_ = value;
  // @@protoc_insertion_point(field_set:proto.Add.add2)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace proto

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_test_2eproto__INCLUDED
