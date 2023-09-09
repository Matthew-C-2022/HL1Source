#include "Netvar.hpp"
#include <format>
void SetupNetvars()
{
	for (auto ClientClazz = M::Client->GetAllClasses();  ClientClazz; ClientClazz = ClientClazz->m_pNext)
	{
		if (ClientClazz->m_pRecvTable)
		{
			Dump(ClientClazz->m_pNetworkName, ClientClazz->m_pRecvTable);
		}
	}

}

void Dump(const char* Baseclass, RecvTable* table, std::uint32_t offset)
{
	for (size_t i = 0; i < table->m_nProps; i++)
	{
		const auto prop = &table->m_pProps[i];




		if (!prop && isdigit(prop->m_pVarName[0]))
			continue;

		if (fnv::Hash(prop->m_pVarName) == fnv::Hash("baseclass"))
		{
			continue;
		}

		if (prop->m_RecvType == SendPropType::DPT_DataTable && prop->m_pDataTable && prop->m_pDataTable->m_pNetTableName[0] == 'D')
		{
			Dump(Baseclass, prop->m_pDataTable, prop->m_Offset + offset);
		}

		const auto hash = std::format( "{}->{}", Baseclass, prop->m_pVarName );
		printf("%s = 0x%X\n", hash.c_str(), offset + prop->m_Offset);
		offsets[fnv::HashConst(hash.c_str())] = offset + prop->m_Offset;
	}


}
