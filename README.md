PE-Crypter
==========

Simple runtime crypter in C/C++. Not FUD.

How to use:

Compile xtea.cpp to create xtea.exe.
Drag & drop file you want to encrypt (put in crypter) on xtea.exe.
This will encrypt the file.

Compile shellcode_generator.c to create shellcode_generator.exe.
Drag & drop file (encrypted with xtea) on shellcode_generator.exe.
This will create shellcode.h which contains the bytes of encrypted file.

Make sure that shellcode.h and runPE.h are in the same directory as file.cpp.
Compile file.cpp to create file.exe.
Running file.exe will decrypt and run file from the first step.

file.exe stores the (encrypted) bytes of an executable in itself, 
when you run it, it decrypts and runs in memory those bytes, by a method called runPE. 
It does not drop anything on the HD. 

Link to starter source code used in this implemenation: http://www.mediafire.com/?qrjxfcqcdvmqo38
