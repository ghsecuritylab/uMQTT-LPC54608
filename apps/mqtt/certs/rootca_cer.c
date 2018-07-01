//#include "wiced_resource.h"
#include <certs/certs.h>
#ifdef AWS
/* AWS CA */
const char rootCa[] = "-----BEGIN CERTIFICATE-----\n" \
"MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n" \
"yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n" \
"ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n" \
"U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n" \
"ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n" \
"aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n" \
"MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n" \
"ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n" \
"biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n" \
"U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n" \
"aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n" \
"nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n" \
"t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n" \
"SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n" \
"BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n" \
"rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n" \
"NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n" \
"BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n" \
"BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n" \
"aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n" \
"MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n" \
"p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n" \
"5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n" \
"WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n" \
"4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n" \
"hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n" \
"-----END CERTIFICATE-----\n";
#else
/* Raspberry pi Ca */
const char rootCa[] = "-----BEGIN CERTIFICATE-----\n"\
"MIIDpzCCAo+gAwIBAgIJAKpi3Twp+3KZMA0GCSqGSIb3DQEBDQUAMGoxFzAVBgNV\n" \
"BAMMDkFuIE1RVFQgYnJva2VyMRYwFAYDVQQKDA1Pd25UcmFja3Mub3JnMRQwEgYD\n" \
"VQQLDAtnZW5lcmF0ZS1DQTEhMB8GCSqGSIb3DQEJARYSbm9ib2R5QGV4YW1wbGUu\n" \
"bmV0MB4XDTE3MTIyOTEyNDA0OFoXDTMyMTIyNTEyNDA0OFowajEXMBUGA1UEAwwO\n" \
"QW4gTVFUVCBicm9rZXIxFjAUBgNVBAoMDU93blRyYWNrcy5vcmcxFDASBgNVBAsM\n" \
"C2dlbmVyYXRlLUNBMSEwHwYJKoZIhvcNAQkBFhJub2JvZHlAZXhhbXBsZS5uZXQw\n" \
"ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDLaygszeT1/VflbZpeRNte\n" \
"InKn1ZfdnbBIBiYYpq6yzBebJhw6d2optQ8VesBrcs0/3YDo5D/N9VhWp6/xLn3Y\n" \
"phxFaXY4JZvYv5WkObfci1eLUZdIB/Z4fTQqbhFLFs/2QfF0xTIySdBifGPyvwNn\n" \
"/Z5IGjkh62lmzg4xkIVTubmaOUtxnSfHv3nSVQnGSIMvsXlvi2HRDbGrSdiwfre9\n" \
"J/zWn8HEu/M+RFmTeStL7iOTNFWkNS7pAunx7BgkjR5C/yu6pkB1GNgVNQ2+ZA/J\n" \
"lJNhf2qj+wVhx2MIt/lhELyybBLr8vdDVy5iV0Le2OLTzYfwiEJr+wSrnnoWmuOr\n" \
"AgMBAAGjUDBOMB0GA1UdDgQWBBQy58AtEHQ0mbbrU1zcdnEGIxYkOzAfBgNVHSME\n" \
"GDAWgBQy58AtEHQ0mbbrU1zcdnEGIxYkOzAMBgNVHRMEBTADAQH/MA0GCSqGSIb3\n" \
"DQEBDQUAA4IBAQAw/tY2cTJGiUkZ49YUFBVu/Mepu4TRFvNamENMKuHxsqDGkYF+\n" \
"VSt4WvDa0MflV96HX9ssJFXWdFd5MeYAa/wjN3H8HdetqdVAxFMfhaMaRgNUYwM5\n" \
"BnI4Ney3EHECNMJLq6jcNdypdbs0/LyLcML30FhXf4tCcV9BnjJsUNG5mBCeSx+l\n" \
"RQHSyoNSeP9jiMJbsI4QnuWXKyttWg7QgpuXH9MTXGyrfBC/feZMcNALPCdJNjj9\n" \
"CbPWnc85uFyXkTPV1kq2EL1qOApHhqRTE7PkT32JZvoSLEEr1vCh9TJ3MwctEZT5\n" \
"XG8uNkqgvhlAgEJ/1V1MryBPRqNbm5gXo0Yq\n" \
"-----END CERTIFICATE-----\n";
#endif
const size_t rootCaSize = sizeof(rootCa);
