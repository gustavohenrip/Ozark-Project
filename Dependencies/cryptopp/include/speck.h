// speck.h - written and placed in the public domain by Jeffrey Walton

//! \file speck.h
//! \brief Classes for the Speck block cipher
//! \details Speck is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
//!   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
//! \sa <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK Families of
//!   Lightweight Block Ciphers</A> and <A HREF="http://iadgov.github.io/simon-speck/">
//!   The Simon and Speck GitHub</A>
//! \since Crypto++ 6.0

#ifndef CRYPTOPP_SPECK_H
#define CRYPTOPP_SPECK_H

#include "config.h"
#include "seckey.h"
#include "secblock.h"

#if CRYPTOPP_BOOL_X64 || CRYPTOPP_BOOL_X32 || CRYPTOPP_BOOL_X86 || CRYPTOPP_BOOL_ARM32 || CRYPTOPP_BOOL_ARM64
# define CRYPTOPP_SPECK_ADVANCED_PROCESS_BLOCKS 1
#endif

NAMESPACE_BEGIN(CryptoPP)

//! \class SPECK_Info
//! \brief SPECK block cipher information
//! \tparam BS block size of the cipher, in bytes
//! \tparam D default key length, in bytes
//! \tparam N minimum key length, in bytes
//! \tparam M maximum key length, in bytes
//! \since Crypto++ 6.0
template <unsigned int BS, unsigned int D, unsigned int N, unsigned int M>
struct SPECK_Info : public FixedBlockSize<BS>, VariableKeyLength<D, N, M>
{
    static const std::string StaticAlgorithmName()
    {
        // Format is Cipher-Blocksize(Keylength)
        return "SPECK-" + IntToString(BS*8);
    }
};

//! \class SPECK_Base
//! \brief SPECK block cipher base class
//! \tparam BS block size of the cipher, in bytes
//! \details User code should use SPECK64 or SPECK128
//! \sa SPECK64, SPECK128, <a href="http://www.cryptopp.com/wiki/SPECK">SPECK</a>
//! \since Crypto++ 6.0
template <class W>
struct SPECK_Base
{
    virtual ~SPECK_Base() {}
    SPECK_Base() : m_kwords(0), m_rounds(0) {}

    typedef SecBlock<W, AllocatorWithCleanup<W, true> > AlignedSecBlock;
    mutable AlignedSecBlock m_wspace;  // workspace
    AlignedSecBlock         m_rkeys;   // round keys
    unsigned int            m_kwords;  // number of key words
    unsigned int            m_rounds;  // number of rounds
};

//! \class SPECK64
//! \brief SPECK 64-bit block cipher
//! \details Speck is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
//!   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
//! \details SPECK64 provides 64-bit block size. The valid key sizes are 96-bit and 128-bit.
//! \sa SPECK64, SPECK128,  <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK
//!   Families of Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
//!   The Simon and Speck GitHub</A>, <a href="http://www.cryptopp.com/wiki/SPECK">SPECK</a> on the
//!   Crypto++ wiki
//! \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SPECK64 : public SPECK_Info<8, 12, 12, 16>, public BlockCipherDocumentation
{
public:
    //! \brief SPECK block cipher transformation functions
    //! \details Provides implementation common to encryption and decryption
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : protected SPECK_Base<word32>, public BlockCipherImpl<SPECK_Info<8, 12, 12, 16> >
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

//! \class SPECK128
//! \brief SPECK 128-bit block cipher
//! \details Speck is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
//!   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
//! \details SPECK128 provides 128-bit block size. The valid key sizes are 128-bit, 192-bit and 256-bit.
//! \sa SPECK64, SPECK128,  <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK
//!   Families of Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
//!   The Simon and Speck GitHub</A>, <a href="http://www.cryptopp.com/wiki/SPECK">SPECK</a> on the
//!   Crypto++ wiki
//! \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SPECK128 : public SPECK_Info<16, 16, 16, 32>, public BlockCipherDocumentation
{
public:
    //! \brief SPECK block cipher transformation functions
    //! \details Provides implementation common to encryption and decryption
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : protected SPECK_Base<word64>, public BlockCipherImpl<SPECK_Info<16, 16, 16, 32> >
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
#if CRYPTOPP_SPECK_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    //! \brief Provides implementation for encryption transformation
    //! \details Dec provides implementation for decryption transformation. All key
    //!   sizes are supported.
    //! \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Dec : public Base
    {
    protected:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
#if CRYPTOPP_SPECK_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    typedef BlockCipherFinal<ENCRYPTION, Enc> Encryption;
    typedef BlockCipherFinal<DECRYPTION, Dec> Decryption;
};

NAMESPACE_END

#endif  // CRYPTOPP_SPECK_H
