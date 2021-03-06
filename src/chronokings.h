#ifndef CHRONOKINGS_H
#define CHRONOKINGS_H

class CNameDB : public CDB
{
protected:
    bool fHaveParent;
public:
    CNameDB(const char* pszMode="r+") : CDB("nameindexfull.dat", pszMode) {
        fHaveParent = false;
    }

    CNameDB(const char* pszMode, CDB& parent) : CDB("nameindexfull.dat", pszMode) {
        vTxn.push_back(parent.GetTxn());
        fHaveParent = true;
    }

    ~CNameDB()
    {
        if (fHaveParent)
            vTxn.erase(vTxn.begin());
    }

    //bool WriteName(std::vector<unsigned char>& name, std::vector<CDiskTxPos> vtxPos)
    bool WriteName(const std::vector<unsigned char>& name, std::vector<CNameIndex>& vtxPos)
    {
        return Write(make_pair(std::string("namei"), name), vtxPos);
    }

    //bool ReadName(std::vector<unsigned char>& name, std::vector<CDiskTxPos>& vtxPos)
    bool ReadName(const std::vector<unsigned char>& name, std::vector<CNameIndex>& vtxPos)
    {
        return Read(make_pair(std::string("namei"), name), vtxPos);
    }

    bool ExistsName(const std::vector<unsigned char>& name)
    {
        return Exists(make_pair(std::string("namei"), name));
    }

    bool EraseName(const std::vector<unsigned char>& name)
    {
        return Erase(make_pair(std::string("namei"), name));
    }

    bool ScanNames(
            const std::vector<unsigned char>& vchName,
            int nMax,
            std::vector<std::pair<std::vector<unsigned char>, CNameIndex> >& nameScan);
            //std::vector<std::pair<std::vector<unsigned char>, CDiskTxPos> >& nameScan);

    bool test();

    bool ReconstructNameIndex();
};

static const int NAMECOIN_TX_VERSION = 0x7100;
static const int64 NAME_COIN_AMOUNT = CENT;
// We can make name_new cheaper, if we want, separately from name_(first)update
// This can be used e.g. to send short messages in the hash field. The coin will be
// destroyed in this case. We can try setting it to 0 though.
static const int64 NAMENEW_COIN_AMOUNT = NAME_COIN_AMOUNT / 5;
static const int MAX_NAME_LENGTH = 255;
static const int MAX_VALUE_LENGTH = 4095;
static const int OP_NAME_INVALID = 0x00;
static const int OP_NAME_NEW = 0x01;
static const int OP_NAME_FIRSTUPDATE = 0x02;
static const int OP_NAME_UPDATE = 0x03;
static const int OP_NAME_NOP = 0x04;
static const int MIN_FIRSTUPDATE_DEPTH = 2;

class CNameIndex;
class CDiskTxPos;
class uint256;

extern std::map<std::vector<unsigned char>, uint256> mapMyNames;
extern std::map<std::vector<unsigned char>, std::set<uint256> > mapNamePending;

std::string stringFromVch(const std::vector<unsigned char> &vch);
std::vector<unsigned char> vchFromString(const std::string &str);
int GetTxPosHeight(const CNameIndex& txPos);
int GetTxPosHeight(const CDiskTxPos& txPos);
int GetTxPosHeight2(const CDiskTxPos& txPos, int nHeight);
bool NameAvailable(CTxDB& txdb, const std::vector<unsigned char> &vchName);
bool GetTxOfName(CNameDB& dbName, const std::vector<unsigned char> &vchName, CTransaction& tx);
bool GetTxOfNameAtHeight(CNameDB& dbName, const std::vector<unsigned char> &vchName, int nHeight, CTransaction& tx);
int IndexOfNameOutput(const CTransaction& tx);
bool GetValueOfTxPos(const CNameIndex& txPos, std::vector<unsigned char>& vchValue, uint256& hash, int& nHeight);
bool GetValueOfTxPos(const CDiskTxPos& txPos, std::vector<unsigned char>& vchValue, uint256& hash, int& nHeight);
bool GetNameOfTx(const CTransaction& tx, std::vector<unsigned char>& name);
bool GetValueOfNameTx(const CTransaction& tx, std::vector<unsigned char>& value);
bool DecodeNameTx(const CTransaction& tx, int& op, int& nOut, std::vector<std::vector<unsigned char> >& vvch);
bool DecodeNameScript(const CScript& script, int& op, std::vector<std::vector<unsigned char> > &vvch);
bool GetNameAddress(const CTransaction& tx, std::string& strAddress);
bool GetNameAddress(const CTransaction& tx, uint160 &hash160);
std::string SendMoneyWithInputTx(CScript scriptPubKey, int64 nValue, int64 nNetFee, CWalletTx& wtxIn, CWalletTx& wtxNew, bool fAskFee);
bool CreateTransactionWithInputTx(const std::vector<std::pair<CScript, int64> >& vecSend, CWalletTx& wtxIn, int nTxOut, CWalletTx& wtxNew, CReserveKey& reservekey, int64& nFeeRet);
int64 GetNetworkFee(int nHeight);
bool IsConflictedTx(CTxDB& txdb, const CTransaction& tx, std::vector<unsigned char>& name);
void UnspendInputs(CWalletTx& wtx);
bool IsPlayerDead(const CWalletTx &nameTx, const CTxIndex &txindex);

#endif // CHRONOKINGS_H
