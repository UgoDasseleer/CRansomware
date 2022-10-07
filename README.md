# CRansomware
A little Ransomware made in C for a school project.

Please do not use it as a virus. It is not made for this purpose. The aim of this project is studying how sockets and crypto work with C.

# How to Use

You need to compilate the ransom.c

/!\ The IP and the port of the remote server (that will receive the key and the IV is hard coded). /!\

On the Client
```
gcc -o ransom ransom.c ransomlib.c -lcrypto
```
On the remote server

You compilate the TCP.C (It will allow you to get the key and IV)
```
gcc TCP.c -o TCP
```

usage of the compilated Binary

```
./TCP <IP> <PORT>
```

/!\ If the TCP server is not running, you won't be able to encrypt files /!\

# Run the Ransomware on the client

To encrypt a whole directory

```
./ransom <path> encrypt
```

To decrypt

```
./ransom <path> decrypt <key> <IV>
```

# Sendback

The send back is used to send back the key and the IV to the client (that will have the TCP scipt compilated on it).

Compilate it 
```
gcc sendback.c -o sendback
```
Run it.
```
./sendback <ip> <port> <key> <IV>
```
