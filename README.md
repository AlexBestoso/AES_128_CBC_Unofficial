# AES_128_CBC_Unofficial

This new update simplifies the main.c file. The AES_128_CBC functions have now been wrapped into one single function that is used to determine the encryption mode. Decryption or encryption.

Results can now be returned from the encyption functions via struct.

Sample usage of the main.c test program:
censored@censored ~/Documents/crypto/AES_128_CBC $ ./make.sh

censored@censored ~/Documents/crypto/AES_128_CBC $ ./aes_128_cbc "$(cat README.md)" -e > test && cat test
f@���*��ʣY���ȣ�_Ԗ�ő�����k�II�sn���cnB÷Ѥ�}�.�C
                                                    ��v��6�*ǐ��_Pw�m������`�l;۬Ä#u<Ú:?x=�?z�o�I��ډ���lM����_}׌e!r�~����`[K�q�pBk����@�xsR�

censored@censored ~/Documents/crypto/AES_128_CBC $ ./aes_128_cbc "$(cat test)" -d
# AES_128_CBC_Unofficial
An unofficial implentation of the CBC mode of operation. Built because I think crypto is cool. use it, but not for production.

Known Issues : 
Decryption fails when processing large files. For example the LICENSE file that comes with this software. 
I beleive the error occurs when calculating the size of the state. Using strlen() will cause errors when the ciphertext contains "\x00". The error seems to be non-existant in the actual encryption/decryption code. 

The fix will likely depend on how the code is implemented. If you're using network sockets, then you'll know the message size via the recv() return value. If you use the sys/stat.h, then you'll also know the message size.

This current implementation is being used to test the returning of cipher and plaintext back to the main function. 
This was being prevented earlier because I didn't define a max buffer size for the state defined in struct AES_128_Struct.

If anyone out there actually has reviewed this code. I thank you for your interest. 
It took a long time to teach myself the mathematics behind this. Even then, I still use hardcoded SBoxes. 
I'm aware that hard coding them is faster than implementing the extended Euclidean algorithm(for software atleast); but I want the satisfaction of fully understanding the mathematics.

Pardon my memoir. >xD>
/**************************************/
An unofficial implentation of the CBC mode of operation. Built because I think crypto is cool. use it, but not for production.
