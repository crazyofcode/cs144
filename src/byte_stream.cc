#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return this->closed_;
}

void Writer::push( string data )
{
  // Your code here.
  if ( data.empty() )
    return;
  uint64_t len = 0;
  uint64_t size = data.size();
  if ( this->bytes_free_ < size ) {
    len = this->bytes_free_;
    data.erase( this->bytes_free_, size - len );
  } else {
    len = size;
  }
  this->buffer_.push_back( data );
  this->bytes_pushed_ += len;
  this->bytes_free_ -= len;
  return;
}

void Writer::close()
{
  // Your code here.
  this->closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return this->bytes_free_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return this->bytes_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return this->closed_ && this->bytes_popped_ == this->bytes_pushed_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return this->bytes_popped_;
}

string_view Reader::peek() const
{
  // Your code here.
  uint64_t len = this->bytes_pushed_ - this->bytes_popped_; // 需要拼接的总长度
  if ( this->buffer_.empty() || len == 0 )
    return string_view( "", 0 );
  static std::string concatenated;
  concatenated.clear(); // 清空之前的拼接结果

  uint64_t total_copied = 0; // 已拼接的字节数

  // 遍历 buffer_ 并拼接字符串
  for ( const std::string& str : this->buffer_ ) {
    uint64_t available = str.size(); // 当前字符串的长度

    // 计算还需要多少字节
    uint64_t to_copy = std::min( len - total_copied, available );

    // 将部分或全部字符串拼接到结果中
    concatenated.append( str.data(), to_copy );

    total_copied += to_copy; // 更新已拼接的字节数

    // 如果已拼接足够的字节，停止遍历
    if ( total_copied >= len )
      break;
  }

  // 返回拼接后的结果
  return std::string_view( concatenated.data(), concatenated.size() );
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  uint64_t tmp = len;
  while ( len > 0 && !this->buffer_.empty() ) {
    string it = this->buffer_.front();
    uint64_t str_len = it.size();
    if ( str_len > len ) {
      this->buffer_.front().erase( 0, len );
      len = 0;
    } else {
      this->buffer_.erase( this->buffer_.begin() );
      len -= str_len;
    }
  }
  this->bytes_popped_ += ( tmp - len );
  this->bytes_free_ += ( tmp - len );
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return this->capacity_ - this->bytes_free_;
}
