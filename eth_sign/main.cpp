#include <libethcore/TransactionBase.h>
#include <libethcore/Common.h>
#include <libethcore/CommonJS.h>
#include <libdevcrypto/DevCryptoCommon.h>
#include <libdevcore/CommonData.h>

using namespace dev;

//testnet chainid set -4  mainnet chainid set 1
std::string trnsfer(std::string to,std::string value,std::string gaslimit,std::string gasprice,std::string data,std::string nonce,std::string priv_key,int chainId =-4) {
	eth::TransactionSkeleton ret;
	//ret.from = jsToAddress("0x085aa94b764316d5e608335d13d926c6c6911e56");
	ret.to = jsToAddress(to);
	ret.value = jsToU256(value);
	ret.gas = jsToU256(gaslimit);
	ret.gasPrice = jsToU256(gasprice);//1GWei
	ret.data = jsToBytes(data);
	ret.nonce = jsToU256(nonce);
	ret.chainId = chainId;


	Secret my_secret(jsToBytes(priv_key));

	eth::TransactionBase trx_base(ret);
	trx_base.sign(my_secret);
	auto ret_data = trx_base.rlp();
	std::string retstr = toHex(ret_data);
	return retstr;
}


std::string ERC20_transfer(std::string contract,std::string to, std::string value, std::string gaslimit, std::string gasprice, std::string data, std::string nonce, std::string priv_key, int chainId = -4) {

	
	eth::TransactionSkeleton ret;
	ret.to = jsToAddress(contract);
	ret.value = jsToU256("0");
	ret.gas = jsToU256(gaslimit);
	ret.gasPrice = jsToU256(gasprice);//1GWei

	auto method_data = sha3("transfer(address,uint256)").asBytes();
	method_data.resize(4);
	auto to_address_data = toBigEndian(jsToU256(to));
	auto value_data = toBigEndian(jsToU256(value));
	method_data.insert(method_data.end(), to_address_data.begin(),to_address_data.end());
	method_data.insert(method_data.end(), value_data.begin(), value_data.end());
	ret.data = method_data;
	std::cout << "data: " << toHex(method_data) << std::endl;
	ret.nonce = jsToU256(nonce);
	ret.chainId = chainId;

	Secret my_secret(jsToBytes(priv_key));

	eth::TransactionBase trx_base(ret);
	trx_base.sign(my_secret);
	auto ret_data = trx_base.rlp();
	std::string retstr = toHex(ret_data);
	return retstr;
}



bool  from_hex(const char *pSrc, std::vector<char> &pDst, unsigned int nSrcLength, unsigned int &nDstLength)
{
	if (pSrc == 0)
	{
		return false;
	}

	nDstLength = 0;

	if (pSrc[0] == 0) // nothing to convert  
		return 0;

	// 计算需要转换的字节数  
	for (int j = 0; pSrc[j]; j++)
	{
		if (isxdigit(pSrc[j]))
			nDstLength++;
	}

	// 判断待转换字节数是否为奇数，然后加一  
	if (nDstLength & 0x01) nDstLength++;
	nDstLength /= 2;

	if (nDstLength > nSrcLength)
		return false;

	nDstLength = 0;

	int phase = 0;
	char temp_char;

	for (int i = 0; pSrc[i]; i++)
	{
		if (!isxdigit(pSrc[i]))
			continue;

		unsigned char val = pSrc[i] - (isdigit(pSrc[i]) ? 0x30 : (isupper(pSrc[i]) ? 0x37 : 0x57));

		if (phase == 0)
		{
			temp_char = val << 4;
			phase++;
		}
		else
		{
			temp_char |= val;
			phase = 0;
			pDst.push_back(temp_char);
			nDstLength++;
		}
	}

	return true;
}
void main(int argv, char* args[]) {
	std::string source = "f86d808504e3b292008344aa209472515816a9f6dd232c87e25be6766d8e7a25ad3b8822b1c8c1227a0000801ba04ef556b8d29c1b69d4c670353aeaa639e362c129fd980e96a02e2d6665ca1e35a076cee2eb8f6aab60ae77524974a44357f1b45937dd87ad6ca0e9586801eeb3d4";
	std::vector<char> temp;
	unsigned int nDeplength = 0;
	bool b_converse = from_hex(source.data(), temp, source.size(), nDeplength);
	dev::bytes bintrx(temp.begin(), temp.end());
	dev::eth::TransactionBase trx_base(bintrx, dev::eth::CheckTransaction::Everything);
	auto retstr = trnsfer("0x38e447b18d9ccbc2334199d58cc009eed30943ea", "1000", "21000", "21000000000","","1048577","0x8f32d0a3227afd02f686a4c82559307c8753e8abe832f1627a19e62b9af7dd0f",-1);
	std::cout << "0x"<<retstr<<std::endl;

	//auto erc20retstr = ERC20_transfer("0x6730b8ff2bcd6485bccb14fd350fbde067b9e508","0x085aa94b764316d5e608335d13d926c6c6911e56", "1000", "500000", "10000000000", "", "6", "0x14d5fe740efc9123befa2b0b5ff954d75fa77cd0320f017729ee2743f1e25ef7",15);
	//std::cout << "ERC20:  0x" << erc20retstr << std::endl;

	system("PAUSE");
}