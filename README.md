Simple-XTEA-Crypter 
==========

Simple runtime crypter in C++. 

How to use:

Compile xtea.cpp to create xtea.exe.
Drag & drop file you want to encrypt (put in crypter) on xtea.exe.
This will encrypt the file and generate shellcode.h.

Make sure that shellcode.h and runPE.h are in the same directory as stub.cpp.
Compile stub.cpp to create stub.exe.
Running stub.exe will decrypt and run file from the first step.

stub.exe stores the (encrypted) bytes of an executable in itself, 
when you run it, it decrypts and runs in memory those bytes, by a method called runPE. 
It does not drop anything on the HD. 

I forked the original because I saw room for improvement and wanted some code for English learners to use. I tried to mark it up with comments as best I can. Technically, if you were able to find a portable c++ compiler, this could be made into a one click solution.  This code should be modified and if you want it to be FUD, you should probably use a different key and a different cipher.

Link to starter source code used in this implementation:
https://github.com/jozemberi/PE-Crypter
Link to starter source code used in original implementation: http://www.mediafire.com/?qrjxfcqcdvmqo38


