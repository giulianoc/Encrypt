/*
 Copyright (C) Giuliano Catrambone (giuliano.catrambone@catrasoftware.it)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 Commercial use other than under the terms of the GNU General Public
 License is allowed only after express negotiation of conditions
 with the authors.
*/

#ifndef Encrypt_h
#define Encrypt_h

#ifdef WIN32
#elif WIN32_OLD

#define _WIN32_WINNT 0x400
#define WINVER 0x400

#include <windows.h>

#define SERVICE_PROVIDER "Microsoft Base Cryptographic Provider v1.0"
#define KEY_CONTAINER "Metallica"
#define NUMBER_ENCRYPT_STRING "�o�s�PQ]"

class clsCryptography
{
  private:
	char *sBuffer;
	char *sBufferCopy;
	DWORD lBufLen;
	char sEncryptedNumber[9];
	HCRYPTPROV hCryptProv;

  public:
	clsCryptography();
	~clsCryptography();

	bool bEncryptData(char *sData, char *sPassword);
	bool bDecryptData(char *sData, char *sPassword);
	char *sGetBuffer();
	long lGetBufferLength();

  private:
	void vResizeBuffer(DWORD lNewLength);
	bool bEncryptDecrypt(char *sData, char *sPassword, bool bEncrypt);
	void vHandleError(char *sError);
	void vAddNumber(long lNumber);
	long lDecryptNumber(char *sNumber);
};
#elif __QTCOMPILER__
#include <QFlags>
#include <QString>
#include <QVector>

/**
 *   @short Simple encryption and decryption of strings and byte arrays
 *
 *   This class provides a simple implementation of encryption and decryption
 *   of strings and byte arrays.
 *
 *   @warning The encryption provided by this class is NOT strong encryption. It may
 *   help to shield things from curious eyes, but it will NOT stand up to someone
 *   determined to break the encryption. Don't say you were not warned.
 *
 *   The class uses a 64 bit key. Simply create an instance of the class, set the key,
 *   and use the encryptToString() method to calculate an encrypted version of the input string.
 *   To decrypt that string again, use an instance of SimpleCrypt initialized with
 *   the same key, and call the decryptToString() method with the encrypted string. If the key
 *   matches, the decrypted version of the string will be returned again.
 *
 *   If you do not provide a key, or if something else is wrong, the encryption and
 *   decryption function will return an empty string or will return a string containing nonsense.
 *   lastError() will return a value indicating if the method was succesful, and if not, why not.
 *
 *   SimpleCrypt is prepared for the case that the encryption and decryption
 *   algorithm is changed in a later version, by prepending a version identifier to the cypertext.
 **/
class SimpleCrypt
{
  public:
	/**
	 *   CompressionMode describes if compression will be applied to the data to be
	 *   encrypted.
	 **/
	enum CompressionMode
	{
		CompressionAuto,   /*!< Only apply compression if that results in a shorter plaintext. */
		CompressionAlways, /*!< Always apply compression. Note that for short inputs, a compression may result in longer data */
		CompressionNever   /*!< Never apply compression. */
	};
	/**
	 *  IntegrityProtectionMode describes measures taken to make it possible to detect problems with the data
	 *  or wrong decryption keys.
	 *
	 *  Measures involve adding a checksum or a cryptograhpic hash to the data to be encrypted. This
	 *  increases the length of the resulting cypertext, but makes it possible to check if the plaintext
	 *  appears to be valid after decryption.
	 **/
	enum IntegrityProtectionMode
	{
		ProtectionNone, /*!< The integerity of the encrypted data is not protected. It is not really possible to detect a wrong key, for instance. */
		ProtectionChecksum, /*!< A simple checksum is used to verify that the data is in order. If not, an empty string is returned. */
		ProtectionHash /*!< A cryptographic hash is used to verify the integrity of the data. This method produces a much stronger, but longer check
						*/
	};
	/**
	 *  Error describes the type of error that occured.
	 **/
	enum Error
	{
		ErrorNoError,		  /*!< No error occurred. */
		ErrorNoKeySet,		  /*!< No key was set. You can not encrypt or decrypt without a valid key. */
		ErrorUnknownVersion,  /*!< The version of this data is unknown, or the data is otherwise not valid. */
		ErrorIntegrityFailed, /*!< The integrity check of the data failed. Perhaps the wrong key was used. */
	};

	/**
	 *   Constructor.
	 *
	 *   Constructs a SimpleCrypt instance without a valid key set on it.
	 **/
	SimpleCrypt();
	/**
	 *  Constructor.
	 *
	 *  Constructs a SimpleCrypt instance and initializes it with the given @arg key.
	 **/
	explicit SimpleCrypt(quint64 key);

	/**
	 *  (Re-) initializes the key with the given @arg key.
	 **/
	void setKey(quint64 key);
	/**
	 *  Returns true if SimpleCrypt has been initialized with a key.
	 **/
	bool hasKey() const { return !m_keyParts.isEmpty(); }

	/**
	 *  Sets the compression mode to use when encrypting data. The default mode is Auto.
	 *
	 *  Note that decryption is not influenced by this mode, as the decryption recognizes
	 *  what mode was used when encrypting.
	 **/
	void setCompressionMode(CompressionMode mode) { m_compressionMode = mode; }
	/**
	 * Returns the CompressionMode that is currently in use.
	 **/
	CompressionMode compressionMode() const { return m_compressionMode; }

	/**
	 *  Sets the integrity mode to use when encrypting data. The default mode is Checksum.
	 *
	 *  Note that decryption is not influenced by this mode, as the decryption recognizes
	 *  what mode was used when encrypting.
	 **/
	void setIntegrityProtectionMode(IntegrityProtectionMode mode) { m_protectionMode = mode; }
	/**
	 *  Returns the IntegrityProtectionMode that is currently in use.
	 **/
	IntegrityProtectionMode integrityProtectionMode() const { return m_protectionMode; }

	/**
	 *  Returns the last error that occurred.
	 **/
	Error lastError() const { return m_lastError; }

	/**
	 *   Encrypts the @arg plaintext string with the key the class was initialized with, and returns
	 *   a cyphertext the result. The result is a base64 encoded version of the binary array that is the
	 *   actual result of the string, so it can be stored easily in a text format.
	 **/
	QString encryptToString(const QString &plaintext);
	/**
	 *  Encrypts the @arg plaintext QByteArray with the key the class was initialized with, and returns
	 *  a cyphertext the result. The result is a base64 encoded version of the binary array that is the
	 *  actual result of the encryption, so it can be stored easily in a text format.
	 **/
	QString encryptToString(QByteArray plaintext);
	/**
	 *  Encrypts the @arg plaintext string with the key the class was initialized with, and returns
	 *  a binary cyphertext in a QByteArray the result.
	 *
	 *  This method returns a byte array, that is useable for storing a binary format. If you need
	 *  a string you can store in a text file, use encryptToString() instead.
	 **/
	QByteArray encryptToByteArray(const QString &plaintext);
	/**
	 *  Encrypts the @arg plaintext QByteArray with the key the class was initialized with, and returns
	 *  a binary cyphertext in a QByteArray the result.
	 *
	 *  This method returns a byte array, that is useable for storing a binary format. If you need
	 *  a string you can store in a text file, use encryptToString() instead.
	 **/
	QByteArray encryptToByteArray(QByteArray plaintext);

	/**
	 *  Decrypts a cyphertext string encrypted with this class with the set key back to the
	 *  plain text version.
	 *
	 *  If an error occured, such as non-matching keys between encryption and decryption,
	 *  an empty string or a string containing nonsense may be returned.
	 **/
	QString decryptToString(const QString &cyphertext);
	/**
	 *  Decrypts a cyphertext string encrypted with this class with the set key back to the
	 *  plain text version.
	 *
	 *  If an error occured, such as non-matching keys between encryption and decryption,
	 *  an empty string or a string containing nonsense may be returned.
	 **/
	QByteArray decryptToByteArray(const QString &cyphertext);
	/**
	 *  Decrypts a cyphertext binary encrypted with this class with the set key back to the
	 *  plain text version.
	 *
	 *  If an error occured, such as non-matching keys between encryption and decryption,
	 *  an empty string or a string containing nonsense may be returned.
	 **/
	QString decryptToString(QByteArray cypher);
	/**
	 *  Decrypts a cyphertext binary encrypted with this class with the set key back to the
	 *  plain text version.
	 *
	 *  If an error occured, such as non-matching keys between encryption and decryption,
	 *  an empty string or a string containing nonsense may be returned.
	 **/
	QByteArray decryptToByteArray(QByteArray cypher);

	// enum to describe options that have been used for the encryption. Currently only one, but
	// that only leaves room for future extensions like adding a cryptographic hash...
	enum CryptoFlag
	{
		CryptoFlagNone = 0,
		CryptoFlagCompression = 0x01,
		CryptoFlagChecksum = 0x02,
		CryptoFlagHash = 0x04
	};
	Q_DECLARE_FLAGS(CryptoFlags, CryptoFlag);

  private:
	void splitKey();

	quint64 m_key;
	QVector<char> m_keyParts;
	CompressionMode m_compressionMode;
	IntegrityProtectionMode m_protectionMode;
	Error m_lastError;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SimpleCrypt::CryptoFlags)

#else
#include <string>

using namespace std;
#endif

class Encrypt
{

  public:
	/*
	 * C++11 method encapsulating all the stuff to encrypt
	 */
	static string encrypt(string stringToBeEncrypted);

	/*
	 * C++11 method encapsulating all the stuff to decrypt
	 */
	static string decrypt(string stringToBeDecrypted);

	/**
crypta un buffer di una qualsiasi lunghezza
input: buffer to encrypt
output: buffer cryptato
ulCryptedBufferLength: bytes allocated through
pCryptedBuffer

Return 0 if SUCCESS, != 0 if fails
*/
	static long encrypt(const char *pBufferToEncrypt, char *pCryptedBuffer, unsigned long ulCryptedBufferLength);

	/**
		decrypta un buffer di una qualsiasi lunghezza
		input: buffer to decrypt
		output: buffer decryptato
		uDecryptedBufferLength: bytes allocated through
			pDecryptedBuffer

		Return 0 if SUCCESS, != 0 if fails
	*/
	static long decrypt(const char *pCryptedBuffer, char *pDecryptedBuffer, unsigned long uDecryptedBufferLength);

	/**
		Return >= 0 if SUCCESS, < 0 if fails
		The returned length does not consider the '\0' char
	*/
	static long getDecryptedBufferLength(const char *pCryptedBuffer);

	/**
		Return >= 0 if SUCCESS, < 0 if fails
		The returned length does not consider the '\0' char
	*/
	static long getCryptedBufferLength(const char *pBufferToEncrypt);

	static string opensslEncrypt(string plaintext);

	static string opensslEncrypt(unsigned char *key, unsigned char *iv, string plaintext);

	static string opensslDecrypt(string base64Encoded);

	static string opensslDecrypt(unsigned char *key, unsigned char *iv, string base64Encoded);

	static string convertFromBinaryToBase64(const unsigned char *buffer, size_t length);

	static size_t calcBinaryLength(const char *b64input);

	static int convertFromBase64ToBinary(const char *b64message, unsigned char **buffer, size_t *length);
};

#endif
