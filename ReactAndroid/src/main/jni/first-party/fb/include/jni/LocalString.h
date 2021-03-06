/*
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <string>

#include <jni.h>

#include <fb/visibility.h>

namespace facebook {
namespace jni {

namespace detail {

void utf8ToModifiedUTF8(const uint8_t* bytes, size_t len, uint8_t* modified, size_t modifiedLength);
size_t modifiedLength(const std::string& str);
size_t modifiedLength(const uint8_t* str, size_t* length);
std::string modifiedUTF8ToUTF8(const uint8_t* modified, size_t len);

}

// JNI represents strings encoded with modified version of UTF-8.  The difference between UTF-8 and
// Modified UTF-8 is that the latter support only 1-byte, 2-byte, and 3-byte formats. Supplementary
// character (4 bytes in unicode) needs to be represented in the form of surrogate pairs. To create
// a Modified UTF-8 surrogate pair that Dalvik would understand we take 4-byte unicode character,
// encode it with UTF-16 which gives us two 2 byte chars (surrogate pair) and then we encode each
// pair as UTF-8. This result in 2 x 3 byte characters.  To convert modified UTF-8 to standard
// UTF-8, this mus tbe reversed.
//
// The second difference is that Modified UTF-8 is encoding NUL byte in 2-byte format.
//
// In order to avoid complex error handling, only a minimum of validity checking is done to avoid
// crashing.  If the input is invalid, the output may be invalid as well.
//
// Relevant links:
//  - http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html
//  - https://docs.oracle.com/javase/6/docs/api/java/io/DataInput.html#modified-utf-8

class FBEXPORT LocalString {
public:
  // Assumes UTF8 encoding and make a required convertion to modified UTF-8 when the string
  // contains unicode supplementary characters.
  explicit LocalString(const std::string& str);
  explicit LocalString(const char* str);
  jstring string() const {
    return m_string;
  }
  ~LocalString();
private:
  jstring m_string;
};

// The string from JNI is converted to standard UTF-8 if the string contains supplementary
// characters.
FBEXPORT std::string fromJString(JNIEnv* env, jstring str);

} }
