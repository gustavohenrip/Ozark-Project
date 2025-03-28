// simon.h - written and placed in the public domain by Jeffrey Walton

//! \file simon.h
//! \brief Classes for the Simon block cipher
//! \details Simon is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
//!   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
//! \sa <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK Families of
//!   Lightweight Block Ciphers</A> and <A HREF="http://iadgov.github.io/simon-speck/">
//!   The Simon and Speck GitHub</A>
//! \since Crypto++ 6.0

#ifndef CRYPTOPP_SIMON_H
#define CRYPTOPP_SIMON_H

#include "config.h"
#include "seckey.h"
#include "secblock.h"

NAMESPACE_BEGIN(CryptoPP)

//! \class SIMON_Info
//! \brief SIMON block cipher information
//! \tparam BS block size of the cipher, in bytes
//! \tparam D default key length, in bytes
//! \tparam N minimum key length, in bytes
//! \tparam M maximum key length, in bytes
//! \since Crypto++ 6.0
template <unsigned int BS, unsigned int D, unsigned int N, unsigned int M>
struct SIMON_Info : public FixedBlockSize<BS>, VariableKeyLength<D, N, M>
{
    static const std::string StaticAlgorithmName()
    {
        // Format is Cipher-Blocksize(Keylength)
        return "SIMON-" + IntToString(BS*8);
    }
};

//! \class SIMON_Base
//! \brief SIMON block cipher base class
//! \tparam BS block size of the cipher, in bytes
//! \details User code should use SIMON64 or SIMON128
//! \sa SIMON64, SIMON128, <a href="http://www.cryptopp.com/wiki/SIMON">SIMON</a> on the Crypto++ wiki
//! \since Crypto++ 6.0
template <class W>
struct SIMON_Base
{
    virtual ~SIMON_Base() {}
    SIMON_Base() : m_kwords(0) {}

    typedef SecBlock<W, AllocatorWithCleanup<W, true> > AlignedSecBlock;
    mutable AlignedSecBlock m_wspace;  // workspace
    AlignedSecBlock         m_rkey;    // round keys
    unsigned int            m_kwords;  // number of key words
};

//! \class SIMON64
//! \brief SIMON 64-bit block cipher
//! \details Simon is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
//!   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
//! \details SIMON64 provides 64-bit block size. The valid key sizes are 96-bit and 128-bit.
//! \sa SIMON64, SIMON128,  <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SIMON
//!   Families of Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
//!   The Simon and Speck GitHub</A>, <a href="http://www.cryptopp.com/wiki/SIMON">SIMON</a> on the
//!   Crypto++ wiki
//! \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SIMON64 : public SIMON_Info<8, 12, 12, 16>, public BlockCipherDocumentation
{
public:
    //! \brief SIMON block cipher transformation functions
    //! \details Provides implementation common to encryption and decryption
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : protected SIMON_Base<word32>, public BlockCipherImpl<SIMON_Info<8, 12, 12, 16> >
    {
    public:
        std::string AlgorithmName() const {
            return StaticAlgorithmName() + (m_kwords == 0 ? "" :
                "(" + IntToString(m_kwords*sizeof(word32)*8) + ")");
        }

    protected:
        void UncheckedSetKey(const byte *userKey, unsigned int keyLength, const NameValuePairs &params);
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

//! \class SIMON128
//! \brief SIMON 128-bit block cipher
//! \details Simon is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
//!   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
//! \details SIMON128 provides 128-bit block size. The valid key sizes are 128-bit, 192-bit and 256-bit.
//! \sa SIMON64, SIMON128,  <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SIMON
//!   Families of Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
//!   The Simon and Speck GitHub</A>, <a href="http://www.cryptopp.com/wiki/SIMON">SIMON</a> on the
//!   Crypto++ wiki
//! \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SIMON128 : public SIMON_Info<16, 16, 16, 32>, public BlockCipherDocumentation
{
public:
    //! \brief SIMON block cipher transformation functions
    //! \details Provides implementation common to encryption and decryption
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : protected SIMON_Base<word64>, public BlockCipherImpl<SIMON_Info<16, 16, 16, 32> >
    {
    public:
        std::string AlgorithmName() const {
            return StaticAlgorithmName() + (m_kwords == 0 ? "" :
                "(" + IntToString(m_kwords*sizeof(word64)*8) + ")");
        }

    protected:
        void UncheckedSetKey(const byte *userKey, unsigned int keyLength, const NameValuePairs &params);
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

#endif  // CRYPTOPP_SIMON_H
