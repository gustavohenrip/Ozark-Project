// sm4.h - written and placed in the public domain by Jeffrey Walton and Han Lulu

//! \file sm4.h
//! \brief Classes for the SM4 block cipher
//! \details SM4 is a Chinese national block cipher. The cipher was formally known as SMS4.
//! \sa <A HREF="http://eprint.iacr.org/2008/329.pdf">SMS4 Encryption Algorithm for Wireless Networks</A>
//! \since Crypto++ 6.0

#ifndef CRYPTOPP_SM4_H
#define CRYPTOPP_SM4_H

#include "config.h"
#include "seckey.h"
#include "secblock.h"

NAMESPACE_BEGIN(CryptoPP)

//! \class SM4_Info
//! \brief SM4 block cipher information
//! \since Crypto++ 6.0
struct SM4_Info : public FixedBlockSize<16>, FixedKeyLength<16>
{
    static const std::string StaticAlgorithmName()
    {
        return "SM4";
    }
};

//! \class SM4
//! \brief Classes for the SM4 block cipher
//! \details SM4 is a Chinese national block cipher. The cipher was formally known as SMS4.
//! \sa <A HREF="http://eprint.iacr.org/2008/329.pdf">SMS4 Encryption Algorithm for Wireless Networks</A>
//! \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SM4 : public SM4_Info, public BlockCipherDocumentation
{
public:
    //! \brief SM4 block cipher transformation functions
    //! \details Provides implementation common to encryption and decryption
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : public BlockCipherImpl<SM4_Info>
    {
    protected:
        void UncheckedSetKey(const byte *userKey, unsigned int keyLength, const NameValuePairs &params);
    protected:
        SecBlock<word32, AllocatorWithCleanup<word32> > m_rkeys;
        mutable SecBlock<word32, AllocatorWithCleanup<word32> > m_wspace;
    };

    //! \brief Provides implementation for encryption transformation
    //! \details Enc provides implementation for encryption transformation. All key
    //!   sizes are supported.
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Enc : public Base
    {
    protected:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
    };

    //! \brief Provides implementation for encryption transformation
    //! \details Dec provides implementation for decryption transformation. All key
    //!   sizes are supported.
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Dec : public Base
    {
    protected:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
    };

    typedef BlockCipherFinal<ENCRYPTION, Enc> Encryption;
    typedef BlockCipherFinal<DECRYPTION, Dec> Decryption;
};

NAMESPACE_END

#endif // CRYPTOPP_SM4_H
