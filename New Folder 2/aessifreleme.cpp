#
#include <iostream>
#include "hex.h"
#include "cryptlib.h"
#include "modes.h"
#include "osrng.h"
#include "aes.h"

#include <iostream>
#include <string>
using namespace std;
using namespace CryptoPP;

struct Key
{
    string iv="11111111111111111111111111111111";;
    string key="0000000000000000";;
};
string convert_to_hex(string& input)
{
    string output;
    CryptoPP::StringSource(input,true,
                           new CryptoPP::HexEncoder(
                               new CryptoPP::StringSink(output)));
    return output;
}
string convert_from_hex(string& hex)
{
    string output;
    CryptoPP::StringSource(hex,true,
                           new CryptoPP::HexEncoder(
                               new CryptoPP::StringSink(output)));
    return output;
}
Key generate_key()
{
    AutoSeededRandomPool rnd;

    SecByteBlock key(0x00,AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(key, key.size());

    SecByteBlock iv(AES::BLOCKSIZE);
    rnd.GenerateBlock(iv, iv.size());

    string keystr((const char*) key.BytePtr(), key.size());
    string ivstr((const char*) iv.BytePtr(), iv.size());

    Key keyout;
    keyout.iv=ivstr;
    keyout.key=keystr;
    return keyout;
}
string encrypt_aes(Key& key, string& plaintext)
{
     CBC_Mode< AES >::Encryption enc;
     const CryptoPP::byte* keydata = (const CryptoPP::byte*) &key.key[0];
     const CryptoPP::byte* ivdata = (const CryptoPP::byte*) &key.iv[0];
     enc.SetKeyWithIV(keydata,key.key.size(),ivdata);

     string cipher;
     StringSource(plaintext,true,
                  new StreamTransformationFilter(
                      enc,new HexEncoder(new StringSink(cipher))));
     return cipher;
}

aessifreleme::aessifreleme()
{

}

std::string aessifreleme::sifre(std::string a)
{
    Key key= generate_key();
    cout<<"Key:"<<convert_to_hex(key.key)<<endl;
    string cipher=encrypt_aes(key,a);
    //cout<<"Encrypted:"<<convert_to_hex(cipher)<<endl;

    return cipher;

}
