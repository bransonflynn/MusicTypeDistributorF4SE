#pragma once

#include "RE/Bethesda/BSFixedString.hpp"
#include "RE/Bethesda/BSLock.hpp"
#include "RE/Bethesda/BSScript/IVMObjectBindInterface.hpp"
#include "RE/Bethesda/BSStorage.hpp"
#include "RE/Bethesda/BSTHashMap.hpp"
#include "RE/Bethesda/BSTSmartPointer.hpp"

namespace RE::BSScript
{
	class BoundScript; // stub
	class IVirtualMachine;
	class MergedBoundScript;
	class Object;
	class Variable;

	struct IHandleReaderWriter;

	namespace UnlinkedTypes
	{
		struct Object; // stub
	}

	class __declspec(novtable) ObjectBindPolicy
	{
	public:
		inline static constexpr auto RTTI{ RTTI::BSScript__ObjectBindPolicy };
		inline static constexpr auto VTABLE{ VTABLE::BSScript__ObjectBindPolicy };

		virtual ~ObjectBindPolicy(); // 00

		// add
		virtual void EnsureBaseDataLoaded(std::size_t a_objHandle) = 0;																																											 // 01
		virtual void ObjectNoLongerNeeded(std::size_t a_objHandle) = 0;																																											 // 02
		virtual void AddBoundInfoImpl(std::size_t a_objHandle) = 0;																																												 // 03
		virtual void ClearBoundInfoImpl(std::size_t a_objHandle) = 0;																																											 // 04
		virtual void ClearDiskLoadedBoundInfoImpl(std::size_t a_objHandle) = 0;																																									 // 05
		virtual void ClearAllBoundInfoImpl() = 0;																																																 // 06
		virtual void PostBindObject(std::size_t a_objHandle) = 0;																																												 // 07
		virtual std::uint32_t GetExtraInfoSize(std::size_t) const = 0;																																											 // 08
		virtual void WriteExtraInfo(std::size_t, const IHandleReaderWriter&, BSStorage&) const = 0;																																				 // 09
		virtual void ReadExtraInfo(std::size_t a_objHandle, std::uint16_t a_handleVersion, const IHandleReaderWriter& a_handleReaderWriter, const BSStorage& a_storage) = 0;																	 // 0A
		virtual bool IsIgnoringClear() const = 0;																																																 // 0B
		virtual void ResolveProperties(std::size_t a_objTarget, const BSTSmartPointer<Object>& a_object, const BSTSmartPointer<BoundScript>& a_boundScript, bool a_postSaveConstOnly) = 0;														 // 0D
		virtual void ResolveProperties(std::size_t a_objTarget, const BSTSmartPointer<Object>& a_object, const MergedBoundScript& a_boundScript, bool a_postSaveConstOnly) = 0;																	 // 0C
		virtual void ConvertProperties(std::size_t a_objTarget, const BSTSmartPointer<BoundScript>& a_boundScript, bool a_constOnly, BSTScrapHashMap<BSFixedString, Variable>& a_properties, std::uint32_t& a_nonConvertedProperties) const = 0; // 0F
		virtual void ConvertProperties(std::size_t a_objTarget, const MergedBoundScript& a_mergedScript, bool a_constOnly, BSTScrapHashMap<BSFixedString, Variable>& a_properties, std::uint32_t& a_nonConvertedProperties) const = 0;			 // 0E

		void BindObject(BSTSmartPointer<Object>& a_obj, std::size_t a_objHandle)
		{
			bindInterface->BindObject(a_obj, a_objHandle, false);
		}

		void UnbindObject(const BSTSmartPointer<Object>& a_obj)
		{
			bindInterface->UnbindObject(a_obj);
		}

		// members
		IVirtualMachine* vm;																		// 10
		IVMObjectBindInterface* bindInterface;														// 18
		mutable BSSpinLock attachedScriptsLock;														// 20
		BSTHashMap<std::size_t, BSTSmallSharedArray<BSTSmartPointer<BoundScript>>> attachedScripts; // 50
	};
	static_assert(sizeof(ObjectBindPolicy) == 0x50);
}
