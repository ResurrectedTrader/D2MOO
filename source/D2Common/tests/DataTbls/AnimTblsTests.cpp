#include <D2CommonTestDefines.h>

#ifdef ANIM_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>
#include <string>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Composit.h>
#include <D2Monsters.h>
#include <DataTbls/AnimTbls.h>
#include <Units/Units.h>

#include <Fixtures/DataTbls/Fixtures.h>


namespace
{
	// Puts a bucket holding a single record (no frame tagged) in the hash table slot of its name, and restores the previous bucket
	struct AnimDataBucketScope
	{
		D2AnimDataTableStrc& anim_data;
		D2AnimDataBucketStrc bucket{};
		D2AnimDataBucketStrc* previous_bucket;
		size_t nHash = 0;

		AnimDataBucketScope(D2AnimDataTableStrc& pAnimData, const char* szName, uint32_t dwFrames)
			: anim_data(pAnimData)
		{
			bucket.nbEntries = 1;
			D2AnimDataRecordStrc& rRecord = record();
			uint8_t nNameHash = 0;
			for (size_t i = 0; szName[i] && i < sizeof(rRecord.szAnimDataName); ++i)
			{
				rRecord.szAnimDataName[i] = static_cast<char>(toupper(szName[i]));
				nNameHash += static_cast<uint8_t>(rRecord.szAnimDataName[i]);
			}
			nHash = nNameHash;
			rRecord.dwFrames = dwFrames;
			rRecord.dwAnimSpeed = 256;

			previous_bucket = anim_data.pHashTableBucket[nHash];
			anim_data.pHashTableBucket[nHash] = &bucket;
		}

		~AnimDataBucketScope()
		{
			anim_data.pHashTableBucket[nHash] = previous_bucket;
		}

		D2AnimDataRecordStrc& record()
		{
			return bucket.aEntries[0];
		}
	};
}


TEST_SUITE("AnimTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD473C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadAnimDataD2, dll_base + 0x000073C0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			const auto moo_result = sut(hArchive);
			const auto original_result = original(hArchive);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD47460" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadAnimDataD2, dll_base + 0x00007460);
		
		SUBCASE("")
		{
			// Input data
			D2AnimDataTableStrc moo_pAnimData{};
			D2AnimDataTableStrc original_pAnimData{};

			const auto setup_data = [](
				D2AnimDataTableStrc& pAnimData
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pAnimData);
			setup_data(original_pAnimData);

			// Call both implementations
			sut(&moo_pAnimData);
			original(&original_pAnimData);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pAnimData, original_pAnimData, "Comparing pAnimData");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD474A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetAnimDataRecord, dll_base + 0x000074A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc original_pUnit{};
			D2InventoryStrc original_pInventory{};
			int nClassId{};
			int nMode{};
			int nUnitType{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2InventoryStrc& pInventory
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pInventory);
			setup_data(original_pUnit, original_pInventory);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nClassId, nMode, nUnitType, &moo_pInventory);
			const auto original_result = original(&original_pUnit, nClassId, nMode, nUnitType, &original_pInventory);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
		}
	}
	
	TEST_CASE_FIXTURE(AnimDataFixture<StatesTxtFixture<PlrModeTypeTxtFixture<ObjModeTypeTxtFixture<MonModeTxtFixture<MonStatsTxtFixture<MonStats2TxtFixture<CharStatsTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>>>>>>>>, "D2Common.0x6FD475D0 (#10640)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnitAnimInfoDebugSet, dll_base + 0x000075D0);

		// A monster without inventory, whose anim data name is built the same way the setter builds it
		D2UnitStrc pUnit{};
		pUnit.dwUnitType = UNIT_MONSTER;
		pUnit.dwClassId = 0;
		pUnit.dwAnimMode = MONMODE_NEUTRAL;

		char szPath[MAX_PATH]{};
		int nWeaponClassCode = 0;
		D2Common_10885_COMPOSIT_unk(&pUnit, szPath, &nWeaponClassCode, 0, 1, nullptr, -1);
		REQUIRE(strlen(szPath) < sizeof(D2AnimDataRecordStrc::szAnimDataName));

		AnimDataBucketScope bucket(*anim_data, szPath, 8);

		// Call both implementations: a matching record is updated and the setter returns
		original(&pUnit, 100);
		const auto original_speed = bucket.record().dwAnimSpeed;

		bucket.record().dwAnimSpeed = 0;
		sut(&pUnit, 100);
		const auto moo_speed = bucket.record().dwAnimSpeed;

		// Compare the updated record
		MOO_CHECK_EQ(moo_speed, original_speed, "Comparing dwAnimSpeed");
	}
	
	TEST_CASE_FIXTURE(AnimDataFixture<NoOpFixture>, "D2Common.0x6FD47700 (#10641)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetAnimDataInfo, dll_base + 0x00007700);

		const auto check_info = [&](const char* szName)
		{
			const std::string name = szName;
			CAPTURE(name);

			// Input data
			char moo_szPath[MAX_PATH]{};
			int moo_pOutLength = -1;
			int moo_pOutAnimSpeed = -1;
			int moo_pOutFirstFrameTagged = -1;
			char original_szPath[MAX_PATH]{};
			int original_pOutLength = -1;
			int original_pOutAnimSpeed = -1;
			int original_pOutFirstFrameTagged = -1;

			strcpy_s(moo_szPath, szName);
			strcpy_s(original_szPath, szName);

			// Call both implementations
			const auto moo_result = sut(moo_szPath, &moo_pOutLength, &moo_pOutAnimSpeed, &moo_pOutFirstFrameTagged);
			const auto original_result = original(original_szPath, &original_pOutLength, &original_pOutAnimSpeed, &original_pOutFirstFrameTagged);

			// Compare return values
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			MOO_CHECK_EQ(moo_szPath, original_szPath, "Comparing szPath");
			MOO_CHECK_EQ(moo_pOutLength, original_pOutLength, "Comparing pOutLength");
			MOO_CHECK_EQ(moo_pOutAnimSpeed, original_pOutAnimSpeed, "Comparing pOutAnimSpeed");
			MOO_CHECK_EQ(moo_pOutFirstFrameTagged, original_pOutFirstFrameTagged, "Comparing pOutFirstFrameTagged");
		};

		SUBCASE("Missing record")
		{
			check_info("l1ophth");
		}

		SUBCASE("Record without a tagged frame")
		{
			// Like AnimData.d2's L1OPHTH: two frames, no frame tagged
			AnimDataBucketScope bucket(*anim_data, "L1OPHTH", 2);
			check_info("l1ophth");
		}

		SUBCASE("Record with a tagged frame")
		{
			AnimDataBucketScope bucket(*anim_data, "L1OPHTH", 12);
			bucket.record().pFrameFlags[5] = 1;
			check_info("l1ophth");
		}
	}
}
#endif
