#pragma once

#include "RE/Bethesda/BSTSmartPointer.hpp"

namespace RE::BSScript::Internal
{
	class __declspec(novtable) IFuncCallQuery
		: public BSIntrusiveRefCounted
	{
	public:
		inline static constexpr auto RTTI{ RTTI::BSScript__Internal__IFuncCallQuery };
		inline static constexpr auto VTABLE{ VTABLE::BSScript__Internal__IFuncCallQuery };
		enum class CallType
		{
			kMember,
			kStatic,
			kGetter,
			kSetter
		};
		virtual ~IFuncCallQuery(); // 00

		// add
		virtual bool GetFunctionCallInfo(
			CallType& a_callType,
			BSTSmartPointer<ObjectTypeInfo>& a_objectTypeInfo,
			BSFixedString& a_name,
			Variable& a_self,
			BSScrapArray<Variable>& a_args) const = 0; // 01

		// members
		std::uint32_t pad0C; // 0C
	};
	static_assert(sizeof(IFuncCallQuery) == 0x10);
}
