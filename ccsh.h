#ifndef CCSH_H
#define CCSH_H

#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

class CCSH {  // Counter ChaCha Stream Hash
 public:
  // Constructors
  CCSH();

  // Public API
  void start(const std::string& data);
  void start(const uint8_t* data, uint64_t length);
  void update(const std::string& data);
  void update(const uint8_t* data, uint64_t length);
  std::string hexdump() const;

 private:
  // Utility Functions
  static void chacha_block(uint32_t out[16], const uint32_t in[16]);
  inline static uint32_t rotateLeft(uint32_t a, uint32_t b);
  static void quarterRound(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d);

  // Member Variables
  std::array<uint32_t, 16> state;  // state of the hash
  uint64_t counter;                // increments all the byte values
  uint64_t nonce;                  // increments each update loop, not every round
  bool initialized;                // flag to track initialization
};

/**
 * @brief Default constructor for CCSH class.
 */
CCSH::CCSH() : state{0}, counter{0}, nonce{0}, initialized{false} {}

/**
 * @brief Initialize the hash with a string.
 *
 * @param data Input string to start the hash computation.
 */
void CCSH::start(const std::string& data) {
  start(reinterpret_cast<const uint8_t*>(data.c_str()), data.length());
}

/**
 * @brief Initialize the hash with a byte array.
 *
 * @param data Pointer to the start of the byte array.
 * @param length Length of the byte array.
 */
void CCSH::start(const uint8_t* data, uint64_t length) {
  state.fill(0);
  counter = 0;
  nonce = 0;
  initialized = false;
  update(data, length);
}

/**
 * @brief Update the hash with additional string data.
 *
 * @param data Input string to update the hash computation.
 */
void CCSH::update(const std::string& data) {
  update(reinterpret_cast<const uint8_t*>(data.c_str()), data.length());
}

/**
 * @brief Update the hash with additional byte data.
 *
 * @param data Pointer to the start of the byte array.
 * @param length Length of the byte array.
 */
void CCSH::update(const uint8_t* data, uint64_t length) {
  std::array<uint32_t, 16> currState = {
    0x65787061,  // expa
    0x6E642033,  // nd 3
    0x32206279,  // 2 by
    0x7465206B,  // te k
  };
  uint32_t* currStatePtr = currState.data();

  std::array<uint32_t, 16> outputBlock{0};

  const uint8_t* dataEnd = data + length;
  for (; data < dataEnd; data += 32) {
    // Reset the required parts of currState
    std::memset(currStatePtr + 4, 0, 8 * sizeof(uint32_t));

    uint8_t bytesToCopy = std::min<uint8_t>(dataEnd - data, 32);
    std::memcpy(currStatePtr + 4, data, bytesToCopy);
    counter += bytesToCopy;

    currState[12] = static_cast<uint32_t>(counter);
    currState[14] = static_cast<uint32_t>(nonce++);

    chacha_block(outputBlock.data(), currState.data());

    if (!initialized) {
      state = outputBlock;
      initialized = true;
    } else {
      for (uint8_t j = 0; j < 16; j++) {
        state[j] ^= outputBlock[j];
      }
    }
  }
}

/**
 * @brief Get a hexdump of the current hash state.
 *
 * @return A string representing the hexdump.
 */
std::string CCSH::hexdump() const {
  std::stringstream ss;
  for (size_t i = 0; i < 16; ++i) {
    ss << std::hex << std::setw(8) << std::setfill('0') << state[i];
  }
  return ss.str();
}

/**
 * @brief ChaCha block computation.
 *
 * @param[out] out Output block.
 * @param[in] in Input block.
 */
void CCSH::chacha_block(uint32_t out[16], const uint32_t in[16]) {
  int i;
  uint32_t x[16];
  for (i = 0; i < 16; ++i) x[i] = in[i];

  for (i = 0; i < 10; i++) {
    quarterRound(x[0], x[4], x[8], x[12]);   // column 0
    quarterRound(x[1], x[5], x[9], x[13]);   // column 1
    quarterRound(x[2], x[6], x[10], x[14]);  // column 2
    quarterRound(x[3], x[7], x[11], x[15]);  // column 3
    quarterRound(x[0], x[5], x[10], x[15]);  // diagonal 1
    quarterRound(x[1], x[6], x[11], x[12]);  // diagonal 2
    quarterRound(x[2], x[7], x[8], x[13]);   // diagonal 3
    quarterRound(x[3], x[4], x[9], x[14]);   // diagonal 4
  }

  for (i = 0; i < 16; ++i) out[i] = x[i] + in[i];
}

/**
 * @brief Rotate a 32-bit number to the left by a specified amount.
 *
 * @param a The number to rotate.
 * @param b Number of positions to rotate.
 * @return Rotated number.
 */
inline uint32_t CCSH::rotateLeft(uint32_t a, uint32_t b) {
    return ((a) << (b)) | ((a) >> (32 - (b)));
}

/**
 * @brief Compute a quarter round for ChaCha.
 *
 * @param a Input/Output 1.
 * @param b Input/Output 2.
 * @param c Input/Output 3.
 * @param d Input/Output 4.
 */
void CCSH::quarterRound(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
  a += b; d ^= a; d = rotateLeft(d, 16);
  c += d; b ^= c; b = rotateLeft(b, 12);
  a += b; d ^= a; d = rotateLeft(d, 8);
  c += d; b ^= c; b = rotateLeft(b, 7);
}
#endif  // CCSH_H
