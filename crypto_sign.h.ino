// crypto_sign.h — Signature ECDSA + SHA-256 (mbedTLS)
#pragma once
#include <mbedtls/sha256.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <Preferences.h>

class CryptoSigner {
private:
  mbedtls_ecdsa_context    _ctx;
  mbedtls_entropy_context  _entropy;
  mbedtls_ctr_drbg_context _ctr_drbg;
  bool _initialized = false;

public:
  CryptoSigner() {
    mbedtls_ecdsa_init(&_ctx);
    mbedtls_entropy_init(&_entropy);
    mbedtls_ctr_drbg_init(&_ctr_drbg);
    mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func,
                           &_entropy, nullptr, 0);
  }

  // Génère + stocke une paire de clés ECDSA secp256k1
  bool generateKey(Preferences& prefs) {
    mbedtls_ecdsa_genkey(&_ctx, MBEDTLS_ECP_DP_SECP256K1,
                          mbedtls_ctr_drbg_random, &_ctr_drbg);
    // Sérialiser et stocker dans NVS chiffré
    uint8_t privBuf[32];
    mbedtls_mpi_write_binary(&_ctx.MBEDTLS_PRIVATE(d), privBuf, 32);
    prefs.putBytes("eckey", privBuf, 32);
    _initialized = true;
    Serial.println("[CRYPTO] Clé ECDSA générée et stockée");
    return true;
  }

  // Charge la clé depuis NVS
  bool loadKey(Preferences& prefs) {
    uint8_t privBuf[32];
    size_t len = prefs.getBytes("eckey", privBuf, 32);
    if (len != 32) return false;
    mbedtls_mpi_read_binary(&_ctx.MBEDTLS_PRIVATE(d), privBuf, 32);
    // Recalculer clé publique
    mbedtls_ecp_mul(&_ctx.grp, &_ctx.Q, &_ctx.MBEDTLS_PRIVATE(d),
                    &_ctx.grp.G, mbedtls_ctr_drbg_random, &_ctr_drbg);
    _initialized = true;
    return true;
  }

  // Hash SHA-256 d'un String → hex string
  String sha256(const String& data) {
    uint8_t hash[32];
    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);
    mbedtls_sha256_starts(&ctx, 0);  // 0 = SHA-256
    mbedtls_sha256_update(&ctx, (const uint8_t*)data.c_str(), data.length());
    mbedtls_sha256_finish(&ctx, hash);
    
    String hex = "";
    for (int i = 0; i < 32; i++) {
      if(hash[i] < 0x10) hex += "0";
      hex += String(hash[i], HEX);
    }
    return hex;
  }

  // Signe un hash hex string → signature DER hex
  String sign(const String& hexHash) {
    if (!_initialized) return "";
    uint8_t hashBytes[32], sig[80];
    size_t sigLen;
    // Décoder hex → bytes
    for(int i=0; i<32; i++)
      hashBytes[i] = strtoul(hexHash.substring(i*2,i*2+2).c_str(), nullptr, 16);
    
    mbedtls_ecdsa_write_signature(&_ctx, MBEDTLS_MD_SHA256,
      hashBytes, 32, sig, sizeof(sig), &sigLen,
      mbedtls_ctr_drbg_random, &_ctr_drbg);
    
    String sigHex = "";
    for(size_t i=0; i<sigLen; i++) {
      if(sig[i]<0x10) sigHex += "0";
      sigHex += String(sig[i], HEX);
    }
    return sigHex;
  }
}