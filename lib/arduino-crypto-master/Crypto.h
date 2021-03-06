/**
 * An extremely minimal crypto library for Arduino devices.
 * 
 * The SHA256 and AES implementations are derived from axTLS 
 * (http://axtls.sourceforge.net/), Copyright (c) 2008, Cameron Rich.
 * 
 * Ported and refactored by Chris Ellis 2016.
 * pkcs7 padding routines added by Mike Killewald Nov 26, 2017 (adopted from https://github.com/spaniakos/AES).
 * 
 */

#ifndef CRYPTO_h
#define CRYPTO_h

#include <Arduino.h>

#if defined ESP8266
#include <osapi.h>
#endif

#define SHA256_SIZE             32
#define SHA256HMAC_SIZE         32
#define SHA256HMAC_BLOCKSIZE    64

/**
 * Compute a SHA256 hash
 */
class SHA256
{
    public:
        SHA256();
        /**
         * Update the hash with new data
         */
        void doUpdate(const byte *msg, int len);
        void doUpdate(const char *msg, unsigned int len) { doUpdate((byte*) msg, len); }
        void doUpdate(const char *msg) { doUpdate((byte*) msg, strlen(msg)); }
        /**
         * Compute the final hash and store it in [digest], digest must be 
         * at least 32 bytes
         */
        void doFinal(byte *digest);
        /**
         * Compute the final hash and check it matches this given expected hash
         */
        bool matches(const byte *expected);
    private:
        void SHA256_Process(const byte digest[64]);
        uint32_t total[2];
        uint32_t state[8];
        uint8_t  buffer[64];
};

#define HMAC_OPAD 0x5C
#define HMAC_IPAD 0x36

/**
 * Compute a HMAC using SHA256
 */
class SHA256HMAC
{
    public:
        /**
         * Compute a SHA256 HMAC with the given [key] key of [length] bytes 
         * for authenticity
         */
        SHA256HMAC(const byte *key, unsigned int keyLen);
        /**
         * Update the hash with new data
         */
        void doUpdate(const byte *msg, unsigned int len);
        void doUpdate(const char *msg, unsigned int len) { doUpdate((byte*) msg, len); }
        void doUpdate(const char *msg) { doUpdate((byte*) msg, strlen(msg)); }
        /**
         * Compute the final hash and store it in [digest], digest must be 
         * at least 32 bytes
         */
        void doFinal(byte *digest);
        /**
         * Compute the final hash and check it matches this given expected hash
         */
        bool matches(const byte *expected);
    private:
        void blockXor(const byte *in, byte *out, byte val, byte len);
        SHA256 _hash;
        byte _innerKey[SHA256HMAC_BLOCKSIZE];
        byte _outerKey[SHA256HMAC_BLOCKSIZE];
};


#if defined ESP8266 || defined ESP32
/**
 * ESP8266 and ESP32 specific true random number generator
 */
class RNG
{
    public:
        /**
         * Fill the [dst] array with [length] random bytes
         */
        static void fill(uint8_t *dst, unsigned int length);
        /**
         * Get a random byte
         */
        static byte get();
        /**
         * Get a 32bit random number
         */
        static uint32_t getLong();
    private:
};
#endif


#endif
