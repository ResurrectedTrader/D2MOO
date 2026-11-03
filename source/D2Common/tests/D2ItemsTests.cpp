#include <D2CommonTestDefines.h>

#ifdef ITEMS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2Inventory.h>
#include <D2Items.h>
#include <D2StatList.h>
#include <GAME/Game.h>
#include <Units/Item.h>
#include <Units/Units.h>

#include <Fixtures/DataTbls/Fixtures.h>


// An item unit for the item query tests
struct ItemQueryTestItem
{
	D2UnitStrc tUnit{};
	D2ItemDataStrc tItemData{};

	ItemQueryTestItem()
	{
		tUnit.dwUnitType = UNIT_ITEM;
		tUnit.pItemData = &tItemData;
	}
};

// An inventory whose items are linked the way the inventory functions link them
struct ItemQueryTestInventory
{
	D2InventoryStrc tInventory{};
	D2InventoryGridStrc tBodyLocGrid{};
	D2UnitStrc* pBodyLocItems[NUM_BODYLOC]{};

	explicit ItemQueryTestInventory(D2UnitStrc* pOwner)
	{
		tInventory.dwSignature = D2C_InventoryHeader;
		tInventory.pOwner = pOwner;
		tBodyLocGrid.nGridWidth = NUM_BODYLOC;
		tBodyLocGrid.nGridHeight = 1;
		tBodyLocGrid.ppItems = pBodyLocItems;
		tInventory.pGrids = &tBodyLocGrid;
		tInventory.nGridCount = 1;
		pOwner->pInventory = &tInventory;
	}

	void add(D2UnitStrc* pItem, int nNodePage)
	{
		D2ItemExtraDataStrc* pExtraData = &pItem->pItemData->pExtraData;
		if (tInventory.pLastItem)
		{
			tInventory.pLastItem->pItemData->pExtraData.pNextItem = pItem;
		}
		else
		{
			tInventory.pFirstItem = pItem;
		}
		pExtraData->pPreviousItem = tInventory.pLastItem;
		pExtraData->pParentInv = &tInventory;
		pExtraData->nNodePosOther = nNodePage;
		tInventory.pLastItem = pItem;
		++tInventory.dwItemCount;
	}

	void equip(D2UnitStrc* pItem, int nBodyLoc)
	{
		add(pItem, NODEPAGE_EQUIP);
		pItem->pItemData->nBodyLoc = nBodyLoc;
		pBodyLocItems[nBodyLoc] = pItem;
	}
};

static int gnItemQueryRepairCallbacks = 0;

static void __fastcall CountItemQueryRepairCallbacks(D2GameStrc*, D2UnitStrc*, D2UnitStrc*)
{
	++gnItemQueryRepairCallbacks;
}


TEST_SUITE("D2ItemsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	// Stat lists are allocated through the original D2Common.dll, so that both implementations can read and change them
	const auto native_AllocStatListEx = reinterpret_cast<decltype(&STATLIST_AllocStatListEx)>(dll_base + 0x00077190);
	const auto native_FreeStatListEx = reinterpret_cast<decltype(&STATLIST_FreeStatListEx)>(dll_base + 0x00077110);
	const auto native_SetStat = reinterpret_cast<decltype(&STATLIST_SetStat)>(dll_base + 0x000777B0);

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98380 (#10687)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AllocItemData, dll_base + 0x00058380);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(moo_pMemPool, &moo_pItem);
			original(original_pMemPool, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD983F0 (#10688)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_FreeItemData, dll_base + 0x000583F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			void* moo_pMemPool = nullptr;
			void* original_pMemPool = nullptr;

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(moo_pMemPool, &moo_pItem);
			original(original_pMemPool, &original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pMemPool, original_pMemPool, "Comparing pMemPool");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98430 (#10689)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetBodyLocation, dll_base + 0x00058430);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98450 (#10690)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetBodyLocation, dll_base + 0x00058450);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint8_t nBodyLoc{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nBodyLoc);
			original(&original_pItem, nBodyLoc);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98470 (#10691)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemSeed, dll_base + 0x00058470);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98490 (#10692)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_InitItemSeed, dll_base + 0x00058490);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem);
			original(&original_pItem);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD984B0 (#10693)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemStartSeed, dll_base + 0x000584B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD984D0 (#10694)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemStartSeed, dll_base + 0x000584D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nSeed{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nSeed);
			original(&original_pItem, nSeed);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98550 (#10695)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemQuality, dll_base + 0x00058550);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98580 (#10696)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemQuality, dll_base + 0x00058580);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nQuality{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nQuality);
			original(&original_pItem, nQuality);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD985A0 (#10699)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetPrefixId, dll_base + 0x000585A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nPrefixNo{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nPrefixNo);
			const auto original_result = original(&original_pItem, nPrefixNo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD985D0 (#10700)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AssignPrefix, dll_base + 0x000585D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint16_t nPrefix{};
			int nPrefixNo{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nPrefix, nPrefixNo);
			original(&original_pItem, nPrefix, nPrefixNo);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98600 (#10697)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAutoAffix, dll_base + 0x00058600);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98630 (#10698)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetAutoAffix, dll_base + 0x00058630);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint16_t nAffix{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nAffix);
			original(&original_pItem, nAffix);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98650 (#10701)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSuffixId, dll_base + 0x00058650);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nSuffixNo{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nSuffixNo);
			const auto original_result = original(&original_pItem, nSuffixNo);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98680 (#10702)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AssignSuffix, dll_base + 0x00058680);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint16_t nSuffix{};
			int nSuffixNo{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nSuffix, nSuffixNo);
			original(&original_pItem, nSuffix, nSuffixNo);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD986B0 (#10703)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetRarePrefixId, dll_base + 0x000586B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD986E0 (#10704)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AssignRarePrefix, dll_base + 0x000586E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint16_t nPrefix{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nPrefix);
			original(&original_pItem, nPrefix);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98700 (#10705)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetRareSuffixId, dll_base + 0x00058700);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98730 (#10706)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AssignRareSuffix, dll_base + 0x00058730);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint16_t nSuffix{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nSuffix);
			original(&original_pItem, nSuffix);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98750 (#10707)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckItemFlag, dll_base + 0x00058750);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint32_t dwFlag{};
			int nLine{};
			char szFile{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, dwFlag, nLine, &szFile);
			const auto original_result = original(&original_pItem, dwFlag, nLine, &szFile);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98780 (#10708)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemFlag, dll_base + 0x00058780);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint32_t dwFlag{};
			BOOL bSet{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, dwFlag, bSet);
			original(&original_pItem, dwFlag, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD987C0 (#10709)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemFlags, dll_base + 0x000587C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD987E0 (#10710)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckItemCMDFlag, dll_base + 0x000587E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nFlag{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nFlag);
			const auto original_result = original(&original_pItem, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98810 (#10711)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemCMDFlag, dll_base + 0x00058810);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nFlag{};
			BOOL bSet{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nFlag, bSet);
			original(&original_pItem, nFlag, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98850 (#10712)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemCMDFlags, dll_base + 0x00058850);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98870 (#10717)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemLevel, dll_base + 0x00058870);

		// The level is raised to 1 with a signed compare, so a negative level becomes 1
		for (const int nItemLevel : { INT_MIN, -1, 0, 1, 99 })
		{
			CAPTURE(nItemLevel);
			ItemQueryTestItem moo_item;
			ItemQueryTestItem original_item;
			moo_item.tItemData.dwItemLevel = nItemLevel;
			original_item.tItemData.dwItemLevel = nItemLevel;

			const auto moo_result = sut(&moo_item.tUnit);
			const auto original_result = original(&original_item.tUnit);
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			MOO_CHECK_EQ(moo_item.tItemData, original_item.tItemData, "Comparing item data");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD988B0 (#10718)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemLevel, dll_base + 0x000588B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nItemLevel{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nItemLevel);
			original(&original_pItem, nItemLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD988E0 (#10719)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetInvPage, dll_base + 0x000588E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98900 (#10720)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetInvPage, dll_base + 0x00058900);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint8_t nPage{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nPage);
			original(&original_pItem, nPage);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98920 (#10721)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetCellOverlap, dll_base + 0x00058920);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98940 (#10722)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetCellOverlap, dll_base + 0x00058940);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nCellOverlap{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nCellOverlap);
			original(&original_pItem, nCellOverlap);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98960 (#10853)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemCell, dll_base + 0x00058960);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98980 (#10854)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemCell, dll_base + 0x00058980);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nItemCell{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nItemCell);
			original(&original_pItem, nItemCell);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD989A0 (#10723)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetEarName, dll_base + 0x000589A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD989C0 (#10724)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetEarName, dll_base + 0x000589C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			char moo_szName{};
			D2UnitStrc original_pItem{};
			char original_szName{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				char& szName
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_szName);
			setup_data(original_pItem, original_szName);

			// Call both implementations
			sut(&moo_pItem, &moo_szName);
			original(&original_pItem, &original_szName);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_szName, original_szName, "Comparing szName");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD989F0 (#10725)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetEarLevel, dll_base + 0x000589F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98A10 (#10726)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetEarLevel, dll_base + 0x00058A10);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint8_t nLevel{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nLevel);
			original(&original_pItem, nLevel);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98A30 (#10727)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetVarGfxIndex, dll_base + 0x00058A30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98A50 (#10728)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetVarGfxIndex, dll_base + 0x00058A50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint8_t nIndex{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nIndex);
			original(&original_pItem, nIndex);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98A70 (#10777)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsRepairable, dll_base + 0x00058A70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98C60 (#10780)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAmmoTypeFromItemType, dll_base + 0x00058C60);
		
		SUBCASE("")
		{
			int nItemType{};

			// Call both implementations
			const auto moo_result = sut(nItemType);
			const auto original_result = original(nItemType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98CA0 (#10781)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAmmoType, dll_base + 0x00058CA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98D20 (#10782)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetQuiverTypeFromItemType, dll_base + 0x00058D20);
		
		SUBCASE("")
		{
			int nItemType{};

			// Call both implementations
			const auto moo_result = sut(nItemType);
			const auto original_result = original(nItemType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98D60 (#10783)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetQuiverType, dll_base + 0x00058D60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98DE0 (#10784)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAutoStackFromItemType, dll_base + 0x00058DE0);
		
		SUBCASE("")
		{
			int nItemType{};

			// Call both implementations
			const auto moo_result = sut(nItemType);
			const auto original_result = original(nItemType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98E20 (#10785)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAutoStack, dll_base + 0x00058E20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98EA0 (#10786)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetReload, dll_base + 0x00058EA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98F20 (#10787)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetReEquip, dll_base + 0x00058F20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD98FA0 (#10788)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetStorePage, dll_base + 0x00058FA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99020 (#10789)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetVarInvGfxCount, dll_base + 0x00059020);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD990A0 (#10790)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetVarInvGfxString, dll_base + 0x000590A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nId{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nId);
			const auto original_result = original(&original_pItem, nId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99140 (#10792)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CanBeRare, dll_base + 0x00059140);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD991C0 (#10791)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CanBeMagic, dll_base + 0x000591C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>, "D2Common.0x6FD99240 (#10793)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CanBeNormal, dll_base + 0x00059240);

		SUBCASE("No item")
		{
			const auto moo_result = sut(nullptr);
			const auto original_result = original(nullptr);
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}

		SUBCASE("Not an item")
		{
			D2UnitStrc tMonster{};
			tMonster.dwUnitType = UNIT_MONSTER;
			const auto moo_result = sut(&tMonster);
			const auto original_result = original(&tMonster);
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}

		SUBCASE("Every item")
		{
			for (int nClassId = 0; nClassId < items_record_count; ++nClassId)
			{
				CAPTURE(nClassId);
				ItemQueryTestItem item;
				item.tUnit.dwClassId = nClassId;
				const auto moo_result = sut(&item.tUnit);
				const auto original_result = original(&item.tUnit);
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD992C0 (#10744)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetWeaponClassCode, dll_base + 0x000592C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD992F0 (#10745)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_Get2HandWeaponClassCode, dll_base + 0x000592F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99370 (#10746)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetBaseCode, dll_base + 0x00059370);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD993F0 (#10747)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAltGfx, dll_base + 0x000593F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99480 (#10748)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetComponent, dll_base + 0x00059480);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99500 (#10749)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetDimensions, dll_base + 0x00059500);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			uint8_t moo_pWidth{};
			uint8_t moo_pHeight{};
			D2UnitStrc original_pItem{};
			uint8_t original_pWidth{};
			uint8_t original_pHeight{};
			char szFile{};
			int nLine{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				uint8_t& pWidth,
				uint8_t& pHeight
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pWidth, moo_pHeight);
			setup_data(original_pItem, original_pWidth, original_pHeight);

			// Call both implementations
			sut(&moo_pItem, &moo_pWidth, &moo_pHeight, &szFile, nLine);
			original(&original_pItem, &original_pWidth, &original_pHeight, &szFile, nLine);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pWidth, original_pWidth, "Comparing pWidth");
			SKIP_MOO_CHECK_EQ(moo_pHeight, original_pHeight, "Comparing pHeight");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99540 (#10750)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAllowedBodyLocations, dll_base + 0x00059540);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			uint8_t moo_pBodyLoc1{};
			uint8_t moo_pBodyLoc2{};
			D2UnitStrc original_pItem{};
			uint8_t original_pBodyLoc1{};
			uint8_t original_pBodyLoc2{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				uint8_t& pBodyLoc1,
				uint8_t& pBodyLoc2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBodyLoc1, moo_pBodyLoc2);
			setup_data(original_pItem, original_pBodyLoc1, original_pBodyLoc2);

			// Call both implementations
			sut(&moo_pItem, &moo_pBodyLoc1, &moo_pBodyLoc2);
			original(&original_pItem, &original_pBodyLoc1, &original_pBodyLoc2);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBodyLoc1, original_pBodyLoc1, "Comparing pBodyLoc1");
			SKIP_MOO_CHECK_EQ(moo_pBodyLoc2, original_pBodyLoc2, "Comparing pBodyLoc2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD995D0 (#10751)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemType, dll_base + 0x000595D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99640 (#10752)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemTypeFromItemId, dll_base + 0x00059640);
		
		SUBCASE("")
		{
			uint32_t dwItemId{};

			// Call both implementations
			const auto moo_result = sut(dwItemId);
			const auto original_result = original(dwItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99680 (#10753)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemQlvl, dll_base + 0x00059680);
		
		SUBCASE("")
		{
			D2UnitStrc pItem{};

			// Call both implementations
			const auto moo_result = sut(&pItem);
			const auto original_result = original(&pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99700 (#10754)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfFlagIsSet, dll_base + 0x00059700);
		
		SUBCASE("")
		{
			int nFlags{};
			int nFlag{};

			// Call both implementations
			const auto moo_result = sut(nFlags, nFlag);
			const auto original_result = original(nFlags, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99710 (#10755)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetOrRemoveFlag, dll_base + 0x00059710);
		
		SUBCASE("")
		{
			// Input data
			int moo_pFlags{};
			int original_pFlags{};
			int nFlag{};
			BOOL bSet{};

			const auto setup_data = [](
				int& pFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pFlags);
			setup_data(original_pFlags);

			// Call both implementations
			sut(&moo_pFlags, nFlag, bSet);
			original(&original_pFlags, nFlag, bSet);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pFlags, original_pFlags, "Comparing pFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99740 (#10756)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckRequirements, dll_base + 0x00059740);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_pUnit{};
			BOOL moo_bStrength{};
			BOOL moo_bDexterity{};
			BOOL moo_bLevel{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_pUnit{};
			BOOL original_bStrength{};
			BOOL original_bDexterity{};
			BOOL original_bLevel{};
			BOOL bEquipping{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2UnitStrc& pUnit,
				BOOL& bStrength,
				BOOL& bDexterity,
				BOOL& bLevel
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pUnit, moo_bStrength, moo_bDexterity, moo_bLevel);
			setup_data(original_pItem, original_pUnit, original_bStrength, original_bDexterity, original_bLevel);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pUnit, bEquipping, &moo_bStrength, &moo_bDexterity, &moo_bLevel);
			const auto original_result = original(&original_pItem, &original_pUnit, bEquipping, &original_bStrength, &original_bDexterity, &original_bLevel);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_bStrength, original_bStrength, "Comparing bStrength");
			SKIP_MOO_CHECK_EQ(moo_bDexterity, original_bDexterity, "Comparing bDexterity");
			SKIP_MOO_CHECK_EQ(moo_bLevel, original_bLevel, "Comparing bLevel");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99BC0 (#10741)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetQuestFromItemId, dll_base + 0x00059BC0);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99C60 (#10742)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetQuest, dll_base + 0x00059C60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99D40 (#10743)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetNormalCode, dll_base + 0x00059D40);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD99DB0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetRequiredLevel, dll_base + 0x00059DB0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_pPlayer{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2UnitStrc& pPlayer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pPlayer);
			setup_data(original_pItem, original_pPlayer);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pPlayer);
			const auto original_result = original(&original_pItem, &original_pPlayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A3F0 (#10757)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetLevelRequirement, dll_base + 0x0005A3F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pUnit);
			setup_data(original_pItem, original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pUnit);
			const auto original_result = original(&original_pItem, &original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A400 (#10758)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckBodyLocation, dll_base + 0x0005A400);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint8_t nBodyLoc{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nBodyLoc);
			const auto original_result = original(&original_pItem, nBodyLoc);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A4F0 (#10762)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckItemTypeIfThrowable, dll_base + 0x0005A4F0);
		
		SUBCASE("")
		{
			int nItemType{};

			// Call both implementations
			const auto moo_result = sut(nItemType);
			const auto original_result = original(nItemType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A530 (#10759)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfThrowable, dll_base + 0x0005A530);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A5B0 (#10760)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetMissileType, dll_base + 0x0005A5B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A5E0 (#10761)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetMeleeRange, dll_base + 0x0005A5E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A610 (#10763)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckWeaponClassByItemId, dll_base + 0x0005A610);
		
		SUBCASE("")
		{
			int nItemId{};
			int nWeapClass{};

			// Call both implementations
			const auto moo_result = sut(nItemId, nWeapClass);
			const auto original_result = original(nItemId, nWeapClass);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A660 (#10764)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckWeaponClass, dll_base + 0x0005A660);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nWeapClass{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nWeapClass);
			const auto original_result = original(&original_pItem, nWeapClass);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A6C0 (#10766)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckWeaponIfTwoHandedByItemId, dll_base + 0x0005A6C0);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A700 (#10765)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckWeaponIfTwoHanded, dll_base + 0x0005A700);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A750 (#10767)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfStackable, dll_base + 0x0005A750);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A7A0 (#10768)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfBeltable, dll_base + 0x0005A7A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A820 (#10769)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_ComparePotionTypes, dll_base + 0x0005A820);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem1{};
			D2UnitStrc moo_pItem2{};
			D2UnitStrc original_pItem1{};
			D2UnitStrc original_pItem2{};

			const auto setup_data = [](
				D2UnitStrc& pItem1,
				D2UnitStrc& pItem2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem1, moo_pItem2);
			setup_data(original_pItem1, original_pItem2);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem1, &moo_pItem2);
			const auto original_result = original(&original_pItem1, &original_pItem2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem1, original_pItem1, "Comparing pItem1");
			SKIP_MOO_CHECK_EQ(moo_pItem2, original_pItem2, "Comparing pItem2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9A960 (#10770)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfAutoBeltable, dll_base + 0x0005A960);
		
		SUBCASE("")
		{
			// Input data
			D2InventoryStrc moo_pInventory{};
			D2UnitStrc moo_pItem{};
			D2InventoryStrc original_pInventory{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2InventoryStrc& pInventory,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pInventory, moo_pItem);
			setup_data(original_pInventory, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pInventory, &moo_pItem);
			const auto original_result = original(&original_pInventory, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pInventory, original_pInventory, "Comparing pInventory");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9AA00 (#10771)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfUseable, dll_base + 0x0005AA00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9AA70 (#10772)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetUniqueColumnFromItemsTxt, dll_base + 0x0005AA70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9AB00 (#10773)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsQuestItem, dll_base + 0x0005AB00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9AB90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CalculateAdditionalCostsForChargedSkills, dll_base + 0x0005AB90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nBaseCost{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nBaseCost);
			const auto original_result = original(&original_pUnit, nBaseCost);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9ACE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CalculateAdditionalCostsForBonusStats, dll_base + 0x0005ACE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			int moo_pSellCost{};
			int moo_pBuyCost{};
			int moo_pRepCost{};
			D2UnitStrc original_pItem{};
			int original_pSellCost{};
			int original_pBuyCost{};
			int original_pRepCost{};
			unsigned int nDivisor{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				int& pSellCost,
				int& pBuyCost,
				int& pRepCost
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pSellCost, moo_pBuyCost, moo_pRepCost);
			setup_data(original_pItem, original_pSellCost, original_pBuyCost, original_pRepCost);

			// Call both implementations
			sut(&moo_pItem, &moo_pSellCost, &moo_pBuyCost, &moo_pRepCost, nDivisor);
			original(&original_pItem, &original_pSellCost, &original_pBuyCost, &original_pRepCost, nDivisor);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pSellCost, original_pSellCost, "Comparing pSellCost");
			SKIP_MOO_CHECK_EQ(moo_pBuyCost, original_pBuyCost, "Comparing pBuyCost");
			SKIP_MOO_CHECK_EQ(moo_pRepCost, original_pRepCost, "Comparing pRepCost");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9B1C0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CalculateTransactionCost, dll_base + 0x0005B1C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pQuestFlags{};
			D2UnitStrc original_pPlayer{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pQuestFlags{};
			D2C_Difficulties nDifficulty{};
			int nVendorId{};
			D2C_TransactionTypes nTransactionType{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2UnitStrc& pItem,
				D2BitBufferStrc& pQuestFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pItem, moo_pQuestFlags);
			setup_data(original_pPlayer, original_pItem, original_pQuestFlags);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, &moo_pItem, nDifficulty, &moo_pQuestFlags, nVendorId, nTransactionType);
			const auto original_result = original(&original_pPlayer, &original_pItem, nDifficulty, &original_pQuestFlags, nVendorId, nTransactionType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pQuestFlags, original_pQuestFlags, "Comparing pQuestFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CB50" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CalculateAdditionalCostsForItemSkill, dll_base + 0x0005CB50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			int moo_pSellCost{};
			int moo_pBuyCost{};
			int moo_pRepCost{};
			D2UnitStrc original_pItem{};
			int original_pSellCost{};
			int original_pBuyCost{};
			int original_pRepCost{};
			unsigned int nDivisor{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				int& pSellCost,
				int& pBuyCost,
				int& pRepCost
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pSellCost, moo_pBuyCost, moo_pRepCost);
			setup_data(original_pItem, original_pSellCost, original_pBuyCost, original_pRepCost);

			// Call both implementations
			sut(&moo_pItem, &moo_pSellCost, &moo_pBuyCost, &moo_pRepCost, nDivisor);
			original(&original_pItem, &original_pSellCost, &original_pBuyCost, &original_pRepCost, nDivisor);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pSellCost, original_pSellCost, "Comparing pSellCost");
			SKIP_MOO_CHECK_EQ(moo_pBuyCost, original_pBuyCost, "Comparing pBuyCost");
			SKIP_MOO_CHECK_EQ(moo_pRepCost, original_pRepCost, "Comparing pRepCost");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CDC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckUnitFlagEx, dll_base + 0x0005CDC0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};
			int nFlag{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, nFlag);
			const auto original_result = original(&original_pUnit, nFlag);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CDE0 (#10775)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetTransactionCost, dll_base + 0x0005CDE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pQuestFlags{};
			D2UnitStrc original_pPlayer{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pQuestFlags{};
			D2C_Difficulties nDifficulty{};
			int nVendorId{};
			D2C_TransactionTypes nTransactionType{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2UnitStrc& pItem,
				D2BitBufferStrc& pQuestFlags
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pItem, moo_pQuestFlags);
			setup_data(original_pPlayer, original_pItem, original_pQuestFlags);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, &moo_pItem, nDifficulty, &moo_pQuestFlags, nVendorId, nTransactionType);
			const auto original_result = original(&original_pPlayer, &original_pItem, nDifficulty, &original_pQuestFlags, nVendorId, nTransactionType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pQuestFlags, original_pQuestFlags, "Comparing pQuestFlags");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CE10 (#10794)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetMaxStack, dll_base + 0x0005CE10);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CE50 (#10795)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetTotalMaxStack, dll_base + 0x0005CE50);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CEF0 (#10798)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSpawnStackFromItemId, dll_base + 0x0005CEF0);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CF30 (#10799)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSpawnStack, dll_base + 0x0005CF30);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CFB0 (#10796)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetMinStackFromItemId, dll_base + 0x0005CFB0);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9CFF0 (#10797)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetMinStack, dll_base + 0x0005CFF0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D0F0 (#10804)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSpellIcon, dll_base + 0x0005D0F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D1E0 (#10805)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetDurWarnCount, dll_base + 0x0005D1E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D260 (#10806)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetQtyWarnCount, dll_base + 0x0005D260);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D2E0 (#10807)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetStrengthBonus, dll_base + 0x0005D2E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D310 (#10808)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetDexBonus, dll_base + 0x0005D310);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D340 (#10809)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfSocketableByItemId, dll_base + 0x0005D340);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D360 (#10810)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckIfSocketable, dll_base + 0x0005D360);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D390 (#10811)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_HasDurability, dll_base + 0x0005D390);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D3F0 (#10813)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetStaffMods, dll_base + 0x0005D3F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D470 (#10814)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAllowedGemSocketsFromItemId, dll_base + 0x0005D470);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>, "D2Common.0x6FD9D490 (#10815)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetMaxSockets, dll_base + 0x0005D490);

		// The item level is read as signed, so a negative level selects the MaxSock1 column
		for (const int nItemLevel : { -1, 1, 26, 41 })
		{
			for (int nClassId = 0; nClassId < items_record_count; ++nClassId)
			{
				CAPTURE(nItemLevel);
				CAPTURE(nClassId);
				ItemQueryTestItem moo_item;
				ItemQueryTestItem original_item;
				moo_item.tUnit.dwClassId = nClassId;
				original_item.tUnit.dwClassId = nClassId;
				moo_item.tItemData.dwItemLevel = nItemLevel;
				original_item.tItemData.dwItemLevel = nItemLevel;

				const auto moo_result = sut(&moo_item.tUnit);
				const auto original_result = original(&original_item.tUnit);
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				MOO_CHECK_EQ(moo_item.tItemData, original_item.tItemData, "Comparing item data");
			}
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D580 (#10816)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSockets, dll_base + 0x0005D580);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D5E0 (#10817)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AddSockets, dll_base + 0x0005D5E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nSockets{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nSockets);
			original(&original_pItem, nSockets);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>, "D2Common.0x6FD9D7C0 (#10818)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetSockets, dll_base + 0x0005D7C0);

		// Covers items of every inventory size, including the small ones (at most 6 cells) whose socket count is capped by their area
		for (const int nSockets : { 2, 6 })
		{
			for (int nClassId = 0; nClassId < items_record_count; ++nClassId)
			{
				CAPTURE(nSockets);
				CAPTURE(nClassId);
				ItemQueryTestItem moo_item;
				ItemQueryTestItem original_item;
				for (ItemQueryTestItem* pItem : { &moo_item, &original_item })
				{
					pItem->tUnit.dwClassId = nClassId;
					pItem->tItemData.dwItemLevel = 41;
					native_AllocStatListEx(&pItem->tUnit, 0, nullptr, nullptr);
				}

				sut(&moo_item.tUnit, nSockets);
				original(&original_item.tUnit, nSockets);

				const auto moo_sockets = STATLIST_UnitGetStatValue(&moo_item.tUnit, STAT_ITEM_NUMSOCKETS, 0);
				const auto original_sockets = STATLIST_UnitGetStatValue(&original_item.tUnit, STAT_ITEM_NUMSOCKETS, 0);
				MOO_CHECK_EQ(moo_item.tItemData.dwItemFlags, original_item.tItemData.dwItemFlags, "Comparing item flags");
				MOO_CHECK_EQ(moo_sockets, original_sockets, "Comparing sockets");

				native_FreeStatListEx(&moo_item.tUnit);
				native_FreeStatListEx(&original_item.tUnit);
			}
		}
	}

	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>, "D2Common.0x6FD9D9E0 (#10822)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetRunesTxtRecordFromItem, dll_base + 0x0005D9E0);

		constexpr int nItemClassId = 0;
		constexpr int nFirstRuneClassId = 1;
		constexpr int nSecondRuneClassId = 2;

		// A complete runeword for the item's own type
		D2RunesTxt tRunesTxt[1]{};
		tRunesTxt[0].nComplete = 1;
		tRunesTxt[0].wIType[0] = items_txt[nItemClassId].wType[0];

		D2RuneDataTbl tPreviousRuneDataTables = sgptDataTables->pRuneDataTables;
		sgptDataTables->pRuneDataTables.nRunesTxtRecordCount = 1;
		sgptDataTables->pRuneDataTables.pRunesTxt = tRunesTxt;

		ItemQueryTestItem item;
		item.tUnit.dwClassId = nItemClassId;
		item.tItemData.dwQualityNo = ITEMQUAL_NORMAL;
		native_AllocStatListEx(&item.tUnit, 0, nullptr, nullptr);
		ItemQueryTestInventory inventory(&item.tUnit);

		ItemQueryTestItem sockets[3];

		SUBCASE("Empty socket inventory")
		{
			// A recipe without runes, on an item without sockets
			const auto moo_result = reinterpret_cast<uintptr_t>(sut(&item.tUnit));
			const auto original_result = reinterpret_cast<uintptr_t>(original(&item.tUnit));
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}

		SUBCASE("Recipe shorter than the socket count")
		{
			// A two-rune recipe matching the first two of three sockets
			tRunesTxt[0].nRune[0] = nFirstRuneClassId;
			tRunesTxt[0].nRune[1] = nSecondRuneClassId;
			sockets[0].tUnit.dwClassId = nFirstRuneClassId;
			sockets[1].tUnit.dwClassId = nSecondRuneClassId;
			sockets[2].tUnit.dwClassId = nSecondRuneClassId;
			for (ItemQueryTestItem& socket : sockets)
			{
				inventory.add(&socket.tUnit, 0);
			}
			native_SetStat(item.tUnit.pStatListEx, STAT_ITEM_NUMSOCKETS, 3, 0);
			const auto moo_result = reinterpret_cast<uintptr_t>(sut(&item.tUnit));
			const auto original_result = reinterpret_cast<uintptr_t>(original(&item.tUnit));
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}

		native_FreeStatListEx(&item.tUnit);
		sgptDataTables->pRuneDataTables = tPreviousRuneDataTables;
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D900 (#10819)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetGemApplyTypeFromItemId, dll_base + 0x0005D900);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D940 (#10820)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetGemApplyType, dll_base + 0x0005D940);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9D9D0 (#10821)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsSocketFiller, dll_base + 0x0005D9D0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9DDD0 (#10803)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CheckBitField1Flag1, dll_base + 0x0005DDD0);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9DE10 (#10802)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsMetalItem, dll_base + 0x0005DE10);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9DE90 (#10774)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsNotQuestItemByItemId, dll_base + 0x0005DE90);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9DEE0 (#10732)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetFileIndex, dll_base + 0x0005DEE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9DF60 (#10733)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetFileIndex, dll_base + 0x0005DF60);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint32_t dwFileIndex{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, dwFileIndex);
			original(&original_pItem, dwFileIndex);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9DFE0 (#11244)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetRealmData, dll_base + 0x0005DFE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			int moo_pRealmData0{};
			int moo_pRealmData1{};
			D2UnitStrc original_pItem{};
			int original_pRealmData0{};
			int original_pRealmData1{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				int& pRealmData0,
				int& pRealmData1
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pRealmData0, moo_pRealmData1);
			setup_data(original_pItem, original_pRealmData0, original_pRealmData1);

			// Call both implementations
			sut(&moo_pItem, &moo_pRealmData0, &moo_pRealmData1);
			original(&original_pItem, &original_pRealmData0, &original_pRealmData1);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pRealmData0, original_pRealmData0, "Comparing pRealmData0");
			SKIP_MOO_CHECK_EQ(moo_pRealmData1, original_pRealmData1, "Comparing pRealmData1");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E070 (#11245)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetRealmData, dll_base + 0x0005E070);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int a2{};
			int a3{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, a2, a3);
			original(&original_pItem, a2, a3);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E0A0 (#10734)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetOwnerId, dll_base + 0x0005E0A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			D2UnitGUID nOwnerGUID{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nOwnerGUID);
			original(&original_pItem, nOwnerGUID);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E120 (#10735)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetOwnerId, dll_base + 0x0005E120);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E1A0 (#10736)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsBodyItem, dll_base + 0x0005E1A0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E2A0 (#10738)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsClassValidByItemId, dll_base + 0x0005E2A0);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E310 (#10737)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsClassValid, dll_base + 0x0005E310);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E390 (#10739)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetClassOfClassSpecificItem, dll_base + 0x0005E390);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E410 (#10823)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetWeaponClassId, dll_base + 0x0005E410);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E480 (#10824)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetTransmogrifyFromItemId, dll_base + 0x0005E480);
		
		SUBCASE("")
		{
			int nItemId{};

			// Call both implementations
			const auto moo_result = sut(nItemId);
			const auto original_result = original(nItemId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E4C0 (#10825)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetTransmogrify, dll_base + 0x0005E4C0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E550 (#10826)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsMagSetRarUniCrfOrTmp, dll_base + 0x0005E550);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E580 (#10740)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsNotQuestItem, dll_base + 0x0005E580);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E5F0 (#10827)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetHitClassFromItem, dll_base + 0x0005E5F0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9E670 (#10828)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_Is1Or2Handed, dll_base + 0x0005E670);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pPlayer{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer, moo_pItem);
			setup_data(original_pPlayer, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, &moo_pItem);
			const auto original_result = original(&original_pPlayer, &original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>, "D2Common.0x6FD9E710 (#10829)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetColor, dll_base + 0x0005E710);

		int nClassId = 0;
		while (nClassId < items_record_count && (items_txt[nClassId].nTransform <= 0 || items_txt[nClassId].nTransform >= 9))
		{
			++nClassId;
		}
		REQUIRE(nClassId < items_record_count);

		// A suffix and a prefix, both with a transform colour
		D2MagicAffixTxt tMagicAffixTxt[2]{};
		D2MagicAffixDataTbl tPreviousMagicAffixDataTables = sgptDataTables->pMagicAffixDataTables;
		sgptDataTables->pMagicAffixDataTables.nMagicAffixTxtRecordCount = 2;
		sgptDataTables->pMagicAffixDataTables.pMagicAffixTxt = tMagicAffixTxt;
		// The original reads the affixes from its own copy of the table
		const auto original_magicaffix_record_count = reinterpret_cast<int*>(dll_base + 0x000A9608 + 0x00000EC8);
		const auto original_magicaffix = reinterpret_cast<D2MagicAffixTxt**>(dll_base + 0x000A9608 + 0x00000ECC);
		const int nPreviousOriginalMagicAffixRecordCount = *original_magicaffix_record_count;
		D2MagicAffixTxt* pPreviousOriginalMagicAffix = *original_magicaffix;
		*original_magicaffix_record_count = 2;
		*original_magicaffix = tMagicAffixTxt;

		ItemQueryTestItem item;
		item.tUnit.dwClassId = nClassId;
		item.tItemData.dwQualityNo = ITEMQUAL_MAGIC;
		item.tItemData.wMagicSuffix[0] = 1;
		item.tItemData.wMagicPrefix[0] = 2;

		const auto compare = [&]()
		{
			for (const int nTransType : { 0, 1 })
			{
				CAPTURE(nTransType);
				uint8_t moo_color = 0xAA;
				uint8_t original_color = 0xAA;
				// The palettes are not loaded, so compare the returned addresses only
				const auto moo_result = reinterpret_cast<uintptr_t>(sut(nullptr, &item.tUnit, &moo_color, nTransType));
				const auto original_result = reinterpret_cast<uintptr_t>(original(nullptr, &item.tUnit, &original_color, nTransType));
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
				MOO_CHECK_EQ(moo_color, original_color, "Comparing color");
			}
		};

		SUBCASE("Colour out of range")
		{
			// Colour bytes are unsigned, so 0xFE is out of range rather than -2
			tMagicAffixTxt[0].nTransformColor = 0xFE;
			compare();
		}

		SUBCASE("No colour")
		{
			// 0xFF marks an affix without a colour, so the prefix colour applies
			tMagicAffixTxt[0].nTransformColor = 0xFF;
			tMagicAffixTxt[1].nTransformColor = 5;
			compare();
		}

		sgptDataTables->pMagicAffixDataTables = tPreviousMagicAffixDataTables;
		*original_magicaffix_record_count = nPreviousOriginalMagicAffixRecordCount;
		*original_magicaffix = pPreviousOriginalMagicAffix;
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9EE70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSetItemsTxtRecord, dll_base + 0x0005EE70);
		
		SUBCASE("")
		{
			int nRecordId{};

			// Call both implementations
			const auto moo_result = sut(nRecordId);
			const auto original_result = original(nRecordId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>, "D2Common.0x6FD9EEA0 (#10830)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsImbueable, dll_base + 0x0005EEA0);

		// Only items below magic quality (or above tempered) can be imbued
		for (int nQuality = ITEMQUAL_INFERIOR; nQuality <= ITEMQUAL_TEMPERED; ++nQuality)
		{
			for (int nClassId = 0; nClassId < items_record_count; ++nClassId)
			{
				CAPTURE(nQuality);
				CAPTURE(nClassId);
				ItemQueryTestItem item;
				item.tUnit.dwClassId = nClassId;
				item.tItemData.dwQualityNo = nQuality;

				const auto moo_result = sut(&item.tUnit);
				const auto original_result = original(&item.tUnit);
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			}
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9F080 (#10832)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsPersonalizable, dll_base + 0x0005F080);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9F260 (#10831)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsSocketable, dll_base + 0x0005F260);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(ItemStatCostTxtFixture<ItemTypesTxtFixture<ItemsTxtFixture<NoOpFixture>>>, "D2Common.0x6FD9F490 (#10877)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetAllRepairCosts, dll_base + 0x0005F490);

		constexpr int nNpcId = 154;
		constexpr int nSkillId = 36;

		uint8_t pQuestFlagBytes[96]{};
		D2BitBufferStrc tQuestFlags{};
		tQuestFlags.pBuffer = pQuestFlagBytes;
		tQuestFlags.nBits = 8 * sizeof(pQuestFlagBytes);

		D2UnitStrc tPlayer{};
		tPlayer.dwUnitType = UNIT_PLAYER;
		ItemQueryTestInventory inventory(&tPlayer);

		// An identified item without durability, holding charges of a skill. As a start item it costs 1 to repair.
		ItemQueryTestItem item;
		item.tUnit.dwClassId = 0;
		item.tItemData.dwQualityNo = ITEMQUAL_NORMAL;
		item.tItemData.dwItemFlags = IFLAG_IDENTIFIED | IFLAG_STARTITEM;
		native_AllocStatListEx(&item.tUnit, 0, nullptr, nullptr);
		inventory.equip(&item.tUnit, BODYLOC_RARM);

		const auto compare = [&]()
		{
			gnItemQueryRepairCallbacks = 0;
			const auto moo_result = sut(nullptr, &tPlayer, nNpcId, DIFFMODE_NORMAL, &tQuestFlags, CountItemQueryRepairCallbacks);
			const auto moo_callbacks = gnItemQueryRepairCallbacks;

			gnItemQueryRepairCallbacks = 0;
			const auto original_result = original(nullptr, &tPlayer, nNpcId, DIFFMODE_NORMAL, &tQuestFlags, CountItemQueryRepairCallbacks);
			const auto original_callbacks = gnItemQueryRepairCallbacks;

			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
			MOO_CHECK_EQ(moo_callbacks, original_callbacks, "Comparing callbacks");
		};

		SUBCASE("Used charge")
		{
			native_SetStat(item.tUnit.pStatListEx, STAT_ITEM_CHARGED_SKILL, (2 << 8) | 1, (nSkillId << 6) | 1);
			compare();
		}

		SUBCASE("Full charges")
		{
			native_SetStat(item.tUnit.pStatListEx, STAT_ITEM_CHARGED_SKILL, (2 << 8) | 2, (nSkillId << 6) | 1);
			compare();
		}

		native_FreeStatListEx(&item.tUnit);
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9F720 (#10833)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_AreStackablesEqual, dll_base + 0x0005F720);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem1{};
			D2UnitStrc moo_pItem2{};
			D2UnitStrc original_pItem1{};
			D2UnitStrc original_pItem2{};

			const auto setup_data = [](
				D2UnitStrc& pItem1,
				D2UnitStrc& pItem2
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem1, moo_pItem2);
			setup_data(original_pItem1, original_pItem2);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem1, &moo_pItem2);
			const auto original_result = original(&original_pItem1, &original_pItem2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem1, original_pItem1, "Comparing pItem1");
			SKIP_MOO_CHECK_EQ(moo_pItem2, original_pItem2, "Comparing pItem2");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FA70 (#10834)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CanItemBeUsedForThrowSkill, dll_base + 0x0005FA70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FB40 (#11079)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(D2COMMON_11079_Return0, dll_base + 0x0005FB40);
		
		SUBCASE("")
		{
			int a1{};
			int a2{};

			// Call both implementations
			const auto moo_result = sut(a1, a2);
			const auto original_result = original(a1, a2);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(SetsTxtFixture<SetItemsTxtFixture<NoOpFixture>>, "D2Common.0x6FD9FB50 (#10836)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSetItemsMask, dll_base + 0x0005FB50);

		D2UnitStrc tPlayer{};
		tPlayer.dwUnitType = UNIT_PLAYER;
		ItemQueryTestInventory inventory(&tPlayer);

		ItemQueryTestItem item;
		item.tItemData.dwQualityNo = ITEMQUAL_SET;
		item.tItemData.dwFileIndex = 0;
		inventory.equip(&item.tUnit, BODYLOC_HEAD);

		const int nPreviousSetItemsTxtRecordCount = sgptDataTables->nSetItemsTxtRecordCount;

		SUBCASE("Set id beyond the set item count")
		{
			// The set's id is looked up in Sets.txt, not in SetItems.txt
			sgptDataTables->nSetItemsTxtRecordCount = 1;
			setitems_txt[0].nSetId = 1;
			setitems_txt[0].nSetItems = 0;
		}

		SUBCASE("Stock set item")
		{
		}

		for (const BOOL bDontIgnoreInputItem : { FALSE, TRUE })
		{
			CAPTURE(bDontIgnoreInputItem);
			const auto moo_result = sut(&tPlayer, &item.tUnit, bDontIgnoreInputItem);
			const auto original_result = original(&tPlayer, &item.tUnit, bDontIgnoreInputItem);
			MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}

		sgptDataTables->nSetItemsTxtRecordCount = nPreviousSetItemsTxtRecordCount;
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FD80 (#10838)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetSetItemsTxtRecordFromItem, dll_base + 0x0005FD80);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FE20 (#10839)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_CanBeEquipped, dll_base + 0x0005FE20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FE70 (#10840)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsCharmUsable, dll_base + 0x0005FE70);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc original_pItem{};
			D2UnitStrc original_pPlayer{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2UnitStrc& pPlayer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pPlayer);
			setup_data(original_pItem, original_pPlayer);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pPlayer);
			const auto original_result = original(&original_pItem, &original_pPlayer);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FF00 (#10776)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetNoOfUnidItems, dll_base + 0x0005FF00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc original_pUnit{};

			const auto setup_data = [](
				D2UnitStrc& pUnit
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit);
			setup_data(original_pUnit);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit);
			const auto original_result = original(&original_pUnit);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FF90 (#10841)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetBonusLifeBasedOnClass, dll_base + 0x0005FF90);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc original_pPlayer{};
			int nValue{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer);
			setup_data(original_pPlayer);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, nValue);
			const auto original_result = original(&original_pPlayer, nValue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD9FFE0 (#10842)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetBonusManaBasedOnClass, dll_base + 0x0005FFE0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pPlayer{};
			D2UnitStrc original_pPlayer{};
			int nValue{};

			const auto setup_data = [](
				D2UnitStrc& pPlayer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pPlayer);
			setup_data(original_pPlayer);

			// Call both implementations
			const auto moo_result = sut(&moo_pPlayer, nValue);
			const auto original_result = original(&original_pPlayer, nValue);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pPlayer, original_pPlayer, "Comparing pPlayer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0030 (#10875)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemFormat, dll_base + 0x00060030);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA00B0 (#10876)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetItemFormat, dll_base + 0x000600B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			uint16_t nItemFormat{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nItemFormat);
			original(&original_pItem, nItemFormat);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0130 (#10878)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetWeaponAttackSpeed, dll_base + 0x00060130);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pWeapon{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pWeapon{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pWeapon
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pWeapon);
			setup_data(original_pUnit, original_pWeapon);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pWeapon);
			const auto original_result = original(&original_pUnit, &original_pWeapon);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pWeapon, original_pWeapon, "Comparing pWeapon");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA02B0 (#10879)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_HasUsedCharges, dll_base + 0x000602B0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			BOOL moo_pHasChargedSkills{};
			D2UnitStrc original_pItem{};
			BOOL original_pHasChargedSkills{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				BOOL& pHasChargedSkills
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pHasChargedSkills);
			setup_data(original_pItem, original_pHasChargedSkills);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pHasChargedSkills);
			const auto original_result = original(&original_pItem, &original_pHasChargedSkills);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pHasChargedSkills, original_pHasChargedSkills, "Comparing pHasChargedSkills");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0340 (#10880)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_IsEthereal, dll_base + 0x00060340);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0370 (#10883)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetCompactItemDataFromBitstream, dll_base + 0x00060370);
		
		SUBCASE("")
		{
			// Input data
			uint8_t moo_pBitstream{};
			D2ItemSaveStrc moo_pItemSave{};
			uint8_t original_pBitstream{};
			D2ItemSaveStrc original_pItemSave{};
			size_t nSize{};
			BOOL bCheckForHeader{};

			const auto setup_data = [](
				uint8_t& pBitstream,
				D2ItemSaveStrc& pItemSave
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pBitstream, moo_pItemSave);
			setup_data(original_pBitstream, original_pItemSave);

			// Call both implementations
			const auto moo_result = sut(&moo_pBitstream, nSize, bCheckForHeader, &moo_pItemSave);
			const auto original_result = original(&original_pBitstream, nSize, bCheckForHeader, &original_pItemSave);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pBitstream, original_pBitstream, "Comparing pBitstream");
			SKIP_MOO_CHECK_EQ(moo_pItemSave, original_pItemSave, "Comparing pItemSave");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0490 (#10882)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_DecodeItemFromBitstream, dll_base + 0x00060490);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			uint8_t moo_pBitstream{};
			int moo_pSocketedItemCount{};
			BOOL moo_pFail{};
			D2UnitStrc original_pItem{};
			uint8_t original_pBitstream{};
			int original_pSocketedItemCount{};
			BOOL original_pFail{};
			size_t nSize{};
			BOOL bCheckForHeader{};
			uint32_t dwVersion{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				uint8_t& pBitstream,
				int& pSocketedItemCount,
				BOOL& pFail
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBitstream, moo_pSocketedItemCount, moo_pFail);
			setup_data(original_pItem, original_pBitstream, original_pSocketedItemCount, original_pFail);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pBitstream, nSize, bCheckForHeader, &moo_pSocketedItemCount, dwVersion, &moo_pFail);
			const auto original_result = original(&original_pItem, &original_pBitstream, nSize, bCheckForHeader, &original_pSocketedItemCount, dwVersion, &original_pFail);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBitstream, original_pBitstream, "Comparing pBitstream");
			SKIP_MOO_CHECK_EQ(moo_pSocketedItemCount, original_pSocketedItemCount, "Comparing pSocketedItemCount");
			SKIP_MOO_CHECK_EQ(moo_pFail, original_pFail, "Comparing pFail");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0620" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_DecodeItemBitstreamCompact, dll_base + 0x00060620);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pBuffer{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pBuffer{};
			BOOL bCheckForHeader{};
			uint32_t dwVersion{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2BitBufferStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBuffer);
			setup_data(original_pItem, original_pBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pBuffer, bCheckForHeader, dwVersion);
			const auto original_result = original(&original_pItem, &original_pBuffer, bCheckForHeader, dwVersion);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA0A20" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_DecodeItemBitstreamComplete, dll_base + 0x00060A20);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pBuffer{};
			int moo_pSocketedItems{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pBuffer{};
			int original_pSocketedItems{};
			BOOL bCheckForHeader{};
			BOOL bGamble{};
			uint32_t dwVersion{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2BitBufferStrc& pBuffer,
				int& pSocketedItems
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBuffer, moo_pSocketedItems);
			setup_data(original_pItem, original_pBuffer, original_pSocketedItems);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pBuffer, bCheckForHeader, bGamble, &moo_pSocketedItems, dwVersion);
			const auto original_result = original(&original_pItem, &original_pBuffer, bCheckForHeader, bGamble, &original_pSocketedItems, dwVersion);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
			SKIP_MOO_CHECK_EQ(moo_pSocketedItems, original_pSocketedItems, "Comparing pSocketedItems");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA2690" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SetDefenseOrDamage, dll_base + 0x00062690);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			int nStat{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			sut(&moo_pItem, nStat);
			original(&original_pItem, nStat);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA29D0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_ReadStatFromItemBitstream, dll_base + 0x000629D0);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pBuffer{};
			D2StatListStrc moo_pStatList{};
			D2ItemStatCostTxt moo_pItemStatCostTxtRecord{};
			D2BitBufferStrc original_pBuffer{};
			D2StatListStrc original_pStatList{};
			D2ItemStatCostTxt original_pItemStatCostTxtRecord{};
			int nStatId{};
			uint32_t dwVersion{};
			int n109{};

			const auto setup_data = [](
				D2BitBufferStrc& pBuffer,
				D2StatListStrc& pStatList,
				D2ItemStatCostTxt& pItemStatCostTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pBuffer, moo_pStatList, moo_pItemStatCostTxtRecord);
			setup_data(original_pBuffer, original_pStatList, original_pItemStatCostTxtRecord);

			// Call both implementations
			sut(&moo_pBuffer, &moo_pStatList, &moo_pItemStatCostTxtRecord, nStatId, dwVersion, n109);
			original(&original_pBuffer, &original_pStatList, &original_pItemStatCostTxtRecord, nStatId, dwVersion, n109);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
			SKIP_MOO_CHECK_EQ(moo_pStatList, original_pStatList, "Comparing pStatList");
			SKIP_MOO_CHECK_EQ(moo_pItemStatCostTxtRecord, original_pItemStatCostTxtRecord, "Comparing pItemStatCostTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA2BA0 (#10881)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SerializeItemToBitstream, dll_base + 0x00062BA0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			uint8_t moo_pBitstream{};
			D2UnitStrc original_pItem{};
			uint8_t original_pBitstream{};
			size_t nSize{};
			BOOL bServer{};
			BOOL bSaveItemInv{};
			BOOL bGamble{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				uint8_t& pBitstream
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBitstream);
			setup_data(original_pItem, original_pBitstream);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pBitstream, nSize, bServer, bSaveItemInv, bGamble);
			const auto original_result = original(&original_pItem, &original_pBitstream, nSize, bServer, bSaveItemInv, bGamble);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBitstream, original_pBitstream, "Comparing pBitstream");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "Inlined in D2Common.0x6FDA2C00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SerializeItemCompact, dll_base + 0x00062C00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pBuffer{};
			D2ItemsTxt moo_pItemsTxtRecord{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pBuffer{};
			D2ItemsTxt original_pItemsTxtRecord{};
			BOOL bServer{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2BitBufferStrc& pBuffer,
				D2ItemsTxt& pItemsTxtRecord
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBuffer, moo_pItemsTxtRecord);
			setup_data(original_pItem, original_pBuffer, original_pItemsTxtRecord);

			// Call both implementations
			sut(&moo_pItem, &moo_pBuffer, &moo_pItemsTxtRecord, bServer);
			original(&original_pItem, &original_pBuffer, &original_pItemsTxtRecord, bServer);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
			SKIP_MOO_CHECK_EQ(moo_pItemsTxtRecord, original_pItemsTxtRecord, "Comparing pItemsTxtRecord");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA2C00" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SerializeItem, dll_base + 0x00062C00);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pBuffer{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pBuffer{};
			BOOL bServer{};
			BOOL bSaveItemInv{};
			BOOL bGamble{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2BitBufferStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBuffer);
			setup_data(original_pItem, original_pBuffer);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, &moo_pBuffer, bServer, bSaveItemInv, bGamble);
			const auto original_result = original(&original_pItem, &original_pBuffer, bServer, bSaveItemInv, bGamble);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA2FD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_WriteBitsToBitstream, dll_base + 0x00062FD0);
		
		SUBCASE("")
		{
			// Input data
			D2BitBufferStrc moo_pBuffer{};
			D2BitBufferStrc original_pBuffer{};
			int nData{};
			int nBits{};

			const auto setup_data = [](
				D2BitBufferStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pBuffer);
			setup_data(original_pBuffer);

			// Call both implementations
			sut(&moo_pBuffer, nData, nBits);
			original(&original_pBuffer, nData, nBits);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA3010" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_SerializeItemComplete, dll_base + 0x00063010);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2BitBufferStrc moo_pBuffer{};
			D2UnitStrc original_pItem{};
			D2BitBufferStrc original_pBuffer{};
			BOOL bServer{};
			BOOL bGamble{};

			const auto setup_data = [](
				D2UnitStrc& pItem,
				D2BitBufferStrc& pBuffer
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem, moo_pBuffer);
			setup_data(original_pItem, original_pBuffer);

			// Call both implementations
			sut(&moo_pItem, &moo_pBuffer, bServer, bGamble);
			original(&original_pItem, &original_pBuffer, bServer, bGamble);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
			SKIP_MOO_CHECK_EQ(moo_pBuffer, original_pBuffer, "Comparing pBuffer");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA42B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetItemStatCostTxtRecord, dll_base + 0x000642B0);
		
		SUBCASE("")
		{
			int nStatId{};

			// Call both implementations
			const auto moo_result = sut(nStatId);
			const auto original_result = original(nStatId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA42E0 (#10837)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_GetNoOfSetItemsFromItem, dll_base + 0x000642E0);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem);
			const auto original_result = original(&original_pItem);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA4380" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDA4380, dll_base + 0x00064380);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pItem{};
			unsigned int nSetItemMask{};

			const auto setup_data = [](
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pItem);
			setup_data(original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pItem, nSetItemMask);
			const auto original_result = original(&original_pItem, nSetItemMask);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA4490" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(sub_6FDA4490, dll_base + 0x00064490);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			int a3{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, a3);
			const auto original_result = original(&original_pUnit, &original_pItem, a3);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FDA4640 (#10866)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(ITEMS_UpdateSets, dll_base + 0x00064640);
		
		SUBCASE("")
		{
			// Input data
			D2UnitStrc moo_pUnit{};
			D2UnitStrc moo_pItem{};
			D2UnitStrc original_pUnit{};
			D2UnitStrc original_pItem{};
			int a3{};
			int a4{};

			const auto setup_data = [](
				D2UnitStrc& pUnit,
				D2UnitStrc& pItem
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pUnit, moo_pItem);
			setup_data(original_pUnit, original_pItem);

			// Call both implementations
			const auto moo_result = sut(&moo_pUnit, &moo_pItem, a3, a4);
			const auto original_result = original(&original_pUnit, &original_pItem, a3, a4);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pUnit, original_pUnit, "Comparing pUnit");
			SKIP_MOO_CHECK_EQ(moo_pItem, original_pItem, "Comparing pItem");
		}
	}
}
#endif
