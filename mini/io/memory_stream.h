#pragma once
#include "stream.h"

namespace mini::io {

class memory_stream
  : public stream
{
  public:
    memory_stream(
      void
      )
      : _begin(nullptr)
      , _end(nullptr)
      , _position(nullptr)
    {

    }

    memory_stream(
      const byte_buffer_ref buffer
      ) // : memory_stream()
    {
      attach(buffer);
    }

    bool
    can_read(
      void
      ) const override
    {
      return true;
    }

    bool
    can_write(
      void
      ) const override
    {
      return true;
    }

    bool
    can_seek(
      void
      ) const override
    {
      return true;
    }

    size_t
    read(
      void* buffer,
      size_t size
      ) override
    {
      size = min(size, (size_t)(_position - _end));
      memory::copy(buffer, _position, size);
      _position += size;
      return size;
    }

    size_t
    write(
      const void* buffer,
      size_t size
      ) override
    {
      size = min(size, (size_t)(_position - _end));
      memory::copy(_position, buffer, size);
      _position += size;
      return size;
    }

    size_t
    seek(
      intptr_t offset,
      seek_origin origin = seek_origin::current
      ) override
    {
      uint8_t* origin_pointer = origin == seek_origin::begin
                  ? _begin    : origin == seek_origin::current
                  ? _position : origin == seek_origin::end
                  ? _end      : nullptr;

      _position = clamp(origin_pointer + offset, _begin, _end);

      return _position - _begin;
    }

    void
    flush(
      void
      ) override
    {
      return;
    }

    void
    attach(
      const byte_buffer_ref buffer
      )
    {
      _begin    = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(buffer.begin()));
      _end      = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(buffer.end()));
      _position = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(buffer.begin()));
    }

    size_t
    get_size(
      void
      ) const
    {
      return _end - _begin;
    }

    size_t
    get_position(
      void
      ) const
    {
      return _position - _begin;
    }

    void*
    get_buffer(
      void
      )
    {
      return _begin;
    }

    const void*
    get_buffer(
      void
      ) const
    {
      return _begin;
    }

  private:
    uint8_t* _begin;
    uint8_t* _end;
    uint8_t* _position;
};

}
