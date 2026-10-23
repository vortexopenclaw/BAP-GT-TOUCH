# Embedded public HTTPS trust store

BAP-GT-TOUCH pins curl's Mozilla-derived October 20, 2026 bundle (118 roots):

- Source: https://curl.se/ca/cacert-2026-10-20.pem
- Published checksum: https://curl.se/ca/cacert-2026-10-20.pem.sha256
- SHA-256: `796f1389bfea13c0ed78f663b765baaff54a508e5483786bd6ece73c5e3e3cbe`
- License: [Mozilla Public License 2.0](https://www.mozilla.org/MPL/2.0/), as documented by [curl](https://curl.se/docs/caextract.html).

This follows the public trust-store policy released in [ESP-Miner v2.15.4 / PR #2040](https://github.com/bitaxeorg/ESP-Miner/pull/2040), following the [eMudhra incident guidance](https://emudhra.we-have-a-problem.com/). The incident concerns intermediate CA keys, not a claim that root private keys were compromised. This snapshot excludes `emSign ECC Root CA - G3` and `AAA Certificate Services`, removing affected direct and cross-signed trust paths. Legitimate servers whose only trust path uses an excluded root will also stop validating. This is not an assertion that a display connection was intercepted.

The Bitcoin price, mempool, and GitHub updater HTTPS clients using `esp_crt_bundle_attach` receive this store without client-code changes. TLS and hostname verification remain enabled. Explicit custom certificate trust, if added separately, is not affected by this public bundle policy.

ESP-IDF custom bundles are **additive**. `sdkconfig` and `sdkconfig.defaults` therefore disable the full, common, and deprecated lists and enable only the pinned bundle. CMake rejects stale settings instead of silently retaining old roots. For an existing checkout, back up `sdkconfig`, then update its certificate-bundle settings to match `sdkconfig.defaults`. Do not delete unrelated settings. Use a clean build after changing trust stores. Deploying the new application is necessary: changing the host's trust store does not update a display.

## Verification

Run with Python 3 and OpenSSL 3:

```sh
python3 tools/test_ca_bundle.py
```

The test verifies input hashes, 118 roots, absence of the two exact DER certificate fingerprints, an accepted ISRG Root X1 control, and untrusted-issuer rejection of all 12 published incident chains. It excludes OpenSSL's system trust directories and store. Time checks are disabled only for offline fixtures, preventing expired certificates from producing false security passes. Firmware time-check policy is unchanged.

The test archive is an unmodified copy of https://emudhra.we-have-a-problem.com/chains.zip, imported from the public ESP-Miner fix. Its SHA-256 is `8a2edc81b6c51e3ad27d45ca1e441ec84c961e2dee0fc547f2a02c0f74315f95`. It contains public certificates, no private keys. The regression test is adapted from ESP-Miner PR #2040.

After an application build, with the ESP-IDF environment sourced:

```sh
python tools/test_ca_bundle.py --build-dir build
```

This regenerates the SDK's compact bundle, compares it byte for byte with the build output, and verifies it is embedded in `lvgl_porting.bin`. Host tests do not establish on-device TLS behavior. Before release, confirm live price, mempool, and update checks on an accessory, and rejection of published incident hosts in a controlled device test. Do not install miner firmware onto the display.

## Updating the snapshot

Download a dated curl snapshot and its separately published checksum. Review upstream trust-policy changes and endpoint compatibility, then update the PEM, checksum, paths in CMake and configuration, expected root count, and regression tests together. Keep incident-chain regression coverage. Builds must not download a mutable trust store. Review separately configured custom CAs on their own trust boundary.
