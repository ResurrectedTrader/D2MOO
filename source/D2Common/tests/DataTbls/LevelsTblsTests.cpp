#include <D2CommonTestDefines.h>

#ifdef LEVELS_TBLS_TESTS

#include <doctest.h>

#include <Windows.h>

#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <string>

#include <TestDefinitions.h>
#include <TestUtilities.h>

#include <D2DataTbls.h>
#include <D2Seed.h>
#include <Storm.h>
#include <DataTbls/LevelsTbls.h>


TEST_SUITE("LevelsTblsTests")
{
	const auto working_directory = std::filesystem::current_path();
	const auto dll_base = reinterpret_cast<uintptr_t>(LoadLibraryA((working_directory / "D2Common.dll").string().c_str()));

	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD5EAE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLevelsTxt, dll_base + 0x0001EAE0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD603C0 (#10631)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLevelsTxtRecord, dll_base + 0x000203C0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD603F0 (#10632)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetRainFromLevelsTxt, dll_base + 0x000203F0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60430 (#10634)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetNoPerFromLevelsTxt, dll_base + 0x00020430);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60470 (#10633)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMudFromLevelsTxt, dll_base + 0x00020470);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD604B0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLevelsTxt, dll_base + 0x000204B0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD604F0 (#11247)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetMonsterLevelInArea, dll_base + 0x000204F0);
		
		SUBCASE("")
		{
			int nLevelId{};
			uint8_t nDifficulty{};
			BOOL bExpansion{};

			// Call both implementations
			const auto moo_result = sut(nLevelId, nDifficulty, bExpansion);
			const auto original_result = original(nLevelId, nDifficulty, bExpansion);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60560" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetPortalLevels, dll_base + 0x00020560);
		
		SUBCASE("")
		{
			// Input data
			int moo_pnPortalLevels{};
			int original_pnPortalLevels{};

			const auto setup_data = [](
				int& pnPortalLevels
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_pnPortalLevels);
			setup_data(original_pnPortalLevels);

			// Call both implementations
			const auto moo_result = sut(&moo_pnPortalLevels);
			const auto original_result = original(&original_pnPortalLevels);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_pnPortalLevels, original_pnPortalLevels, "Comparing pnPortalLevels");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60570" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLevelDefsBin, dll_base + 0x00020570);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60D60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLevelDefsBin, dll_base + 0x00020D60);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60D90 (#10010)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLevelDefRecord, dll_base + 0x00020D90);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD60DC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLevelTypesTxt, dll_base + 0x00020DC0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61450" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLevelTypesTxt, dll_base + 0x00021450);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61460 (#10023)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLevelTypesTxtRecord, dll_base + 0x00021460);
		
		SUBCASE("")
		{
			int nLevelType{};

			// Call both implementations
			const auto moo_result = sut(nLevelType);
			const auto original_result = original(nLevelType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD614A0 (#11226)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_CheckActInLevelTypesTxt, dll_base + 0x000214A0);
		
		SUBCASE("")
		{
			int nLevelType{};
			uint8_t nAct{};

			// Call both implementations
			const auto moo_result = sut(nLevelType, nAct);
			const auto original_result = original(nLevelType, nAct);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61500 (#11227)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetFileNameFromLevelTypeAndFileId, dll_base + 0x00021500);
		
		SUBCASE("")
		{
			// Input data
			char moo_szFile{};
			char original_szFile{};
			int nLevelType{};
			int nFile{};

			const auto setup_data = [](
				char& szFile
			) {
				// TODO: Setup as needed
			};

			setup_data(moo_szFile);
			setup_data(original_szFile);

			// Call both implementations
			sut(nLevelType, nFile, &moo_szFile);
			original(nLevelType, nFile, &original_szFile);

			// Compare potentially modified input data
			SKIP_MOO_CHECK_EQ(moo_szFile, original_szFile, "Comparing szFile");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61570" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlPrestTxt, dll_base + 0x00021570);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};
			int a2{};

			// Call both implementations
			sut(hArchive, a2);
			original(hArchive, a2);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61AD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlPrestTxt, dll_base + 0x00021AD0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61B50 (#10024)" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlPrestTxtRecord, dll_base + 0x00021B50);
		
		SUBCASE("")
		{
			int nId{};

			// Call both implementations
			const auto moo_result = sut(nId);
			const auto original_result = original(nId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61B80" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlPrestTxtRecordFromLevelId, dll_base + 0x00021B80);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61BC0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlWarpTxt, dll_base + 0x00021BC0);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61DD0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlWarpTxt, dll_base + 0x00021DD0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61DE0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlWarpTxtRecordFromLevelIdAndDirection, dll_base + 0x00021DE0);
		
		SUBCASE("")
		{
			int nLevelId{};
			char szDirection{};

			// Call both implementations
			const auto moo_result = sut(nLevelId, szDirection);
			const auto original_result = original(nLevelId, szDirection);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61E60" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlMazeTxt, dll_base + 0x00021E60);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};

			// Call both implementations
			sut(hArchive);
			original(hArchive);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61F90" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlMazeTxt, dll_base + 0x00021F90);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD61FA0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlMazeTxtRecordFromLevelId, dll_base + 0x00021FA0);
		
		SUBCASE("")
		{
			int nLevelId{};

			// Call both implementations
			const auto moo_result = sut(nLevelId);
			const auto original_result = original(nLevelId);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62020" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadLvlSubTxt, dll_base + 0x00022020);
		
		SUBCASE("")
		{
			HD2ARCHIVE hArchive{};
			int a2{};
			int a3{};

			// Call both implementations
			sut(hArchive, a2, a3);
			original(hArchive, a2, a3);
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62600" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_UnloadLvlSubTxt, dll_base + 0x00022600);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD626F0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetLvlSubTxtRecord, dll_base + 0x000226F0);
		
		SUBCASE("")
		{
			int nSubType{};

			// Call both implementations
			const auto moo_result = sut(nSubType);
			const auto original_result = original(nSubType);
			
			// Compare return values
			SKIP_MOO_CHECK_EQ(moo_result, original_result, "Comparing results");
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62780" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_AllocGlobalTileLibraryHash, dll_base + 0x00022780);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD628A0" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_FreeGlobalTileLibraryHash, dll_base + 0x000228A0);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD628C0")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_LoadAutomapTxt, dll_base + 0x000228C0);

		const auto original_data_tables = reinterpret_cast<D2DataTablesStrc*>(dll_base + 0x000A9608);

		// automap.bin with a leading row whose LevelName and TileName are the '0' placeholders, read from a separate directory
		auto [record_count, automap] = read_bin_file<D2AutomapTxt>("automap.bin");
		REQUIRE(record_count > 0);

		D2AutomapTxt placeholder = automap[0];
		memset(placeholder.szLevelName, 0, sizeof(placeholder.szLevelName));
		memset(placeholder.szTileName, 0, sizeof(placeholder.szTileName));
		placeholder.szLevelName[0] = '0';
		placeholder.szTileName[0] = '0';

		const auto previous_directory = std::filesystem::current_path();
		const auto data_directory = previous_directory / "automap_placeholder";
		std::filesystem::remove_all(data_directory);
		std::filesystem::create_directories(data_directory / "data/global/excel");
		{
			std::ofstream file((data_directory / "data/global/excel/automap.bin").string(), std::ios::binary);
			const int placeholder_record_count = record_count + 1;
			file.write(reinterpret_cast<const char*>(&placeholder_record_count), sizeof(placeholder_record_count));
			file.write(reinterpret_cast<const char*>(&placeholder), sizeof(placeholder));
			file.write(reinterpret_cast<const char*>(automap.get()), sizeof(D2AutomapTxt) * record_count);
		}

		D2AutomapShortStrc* const moo_previous_automap = sgptDataTables->pAutoMap;
		const int moo_previous_automap_count = sgptDataTables->nAutoMap;
		D2AutomapShortStrc* const original_previous_automap = original_data_tables->pAutoMap;
		const int original_previous_automap_count = original_data_tables->nAutoMap;
		D2AutomapRandStrc moo_previous_rand[ARRAY_SIZE(sgptDataTables->pAutoMapRand)];
		D2AutomapRandStrc original_previous_rand[ARRAY_SIZE(sgptDataTables->pAutoMapRand)];
		memcpy(moo_previous_rand, sgptDataTables->pAutoMapRand, sizeof(moo_previous_rand));
		memcpy(original_previous_rand, original_data_tables->pAutoMapRand, sizeof(original_previous_rand));

		// Files are read relative to Storm's base path
		char szPreviousBasePath[MAX_PATH]{};
		SFileGetBasePath(szPreviousBasePath, sizeof(szPreviousBasePath));
		std::string szBasePath = data_directory.string() + "\\";
		SFileSetBasePath(szBasePath.data());

		// Call both implementations
		sut(nullptr);
		original(nullptr);

		SFileSetBasePath(szPreviousBasePath);

		// Compare the loaded tables
		REQUIRE(sgptDataTables->nAutoMap == record_count + 1);
		MOO_CHECK_EQ(sgptDataTables->nAutoMap, original_data_tables->nAutoMap, "Comparing nAutoMap");
		for (int i = 0; i < sgptDataTables->nAutoMap; ++i)
		{
			CAPTURE(i);
			MOO_CHECK_EQ(sgptDataTables->pAutoMap[i], original_data_tables->pAutoMap[i], "Comparing pAutoMap");
		}
		for (int i = 0; i < ARRAY_SIZE(sgptDataTables->pAutoMapRand); ++i)
		{
			CAPTURE(i);
			MOO_CHECK_EQ(sgptDataTables->pAutoMapRand[i], original_data_tables->pAutoMapRand[i], "Comparing pAutoMapRand");
		}

		D2_FREE_POOL(nullptr, sgptDataTables->pAutoMap);
		sgptDataTables->pAutoMap = moo_previous_automap;
		sgptDataTables->nAutoMap = moo_previous_automap_count;
		original_data_tables->pAutoMap = original_previous_automap;
		original_data_tables->nAutoMap = original_previous_automap_count;
		memcpy(sgptDataTables->pAutoMapRand, moo_previous_rand, sizeof(moo_previous_rand));
		memcpy(original_data_tables->pAutoMapRand, original_previous_rand, sizeof(original_previous_rand));
		std::filesystem::remove_all(data_directory);
	}
	
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62D30 (#10011)")
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_GetAutomapCellId, dll_base + 0x00022D30);

		// The original reads its own data tables and automap seed directly
		const auto original_data_tables = reinterpret_cast<D2DataTablesStrc*>(dll_base + 0x000A9608);
		const auto original_automap_seed = *reinterpret_cast<D2SeedStrc**>(dll_base + 0x0009AF34);

		constexpr uint32_t dwAutomapLevelType = 1;
		constexpr uint32_t dwAutomapTileType = 0;
		constexpr int nStyle = 45;

		// automap.txt writes an open-ended sequence range as EndSequence -1; a Style or StartSequence of -1 is a wildcard
		const auto make_row = [&](char nRowStyle, char nStartSequence, char nEndSequence)
		{
			D2AutomapShortStrc row{};
			row.dwAutomapLevelType = dwAutomapLevelType;
			row.dwAutomapTileType = dwAutomapTileType;
			row.nStyle = nRowStyle;
			row.nStartSequence = nStartSequence;
			row.nEndSequence = nEndSequence;
			row.nCel[0] = 17;
			row.nCel[1] = 18;
			row.nCels = 2;
			return row;
		};

		const D2AutomapShortStrc rows[] = {
			make_row(nStyle, 7, -1),
			make_row(nStyle, 0, -1),
			make_row(nStyle, static_cast<char>(0x80), -1),
			make_row(nStyle, 7, 20),
			make_row(-1, -1, 0),
		};

		const D2AutomapRandStrc moo_previous_rand = sgptDataTables->pAutoMapRand[dwAutomapLevelType];
		const D2AutomapRandStrc original_previous_rand = original_data_tables->pAutoMapRand[dwAutomapLevelType];
		D2AutomapShortStrc* const moo_previous_automap = sgptDataTables->pAutoMap;
		D2AutomapShortStrc* const original_previous_automap = original_data_tables->pAutoMap;
		const D2SeedStrc moo_previous_seed = sgptDataTables->pAutomapSeed;
		const D2SeedStrc original_previous_seed = *original_automap_seed;

		sgptDataTables->pAutoMapRand[dwAutomapLevelType] = { 0, 1 };
		original_data_tables->pAutoMapRand[dwAutomapLevelType] = { 0, 1 };

		for (int nRow = 0; nRow < ARRAY_SIZE(rows); ++nRow)
		{
			D2AutomapShortStrc moo_row = rows[nRow];
			D2AutomapShortStrc original_row = rows[nRow];
			sgptDataTables->pAutoMap = &moo_row;
			original_data_tables->pAutoMap = &original_row;

			for (const int nSequence : { 0, 6, 7, 10, 20, 21, 127, 128, 200, 255 })
			{
				CAPTURE(nRow);
				CAPTURE(nSequence);

				sgptDataTables->pAutomapSeed = { 0, 666 };
				*original_automap_seed = { 0, 666 };

				// Call both implementations
				const auto moo_result = sut(dwAutomapLevelType, dwAutomapTileType, nStyle, nSequence);
				const auto original_result = original(dwAutomapLevelType, dwAutomapTileType, nStyle, nSequence);

				// Compare return values
				MOO_CHECK_EQ(moo_result, original_result, "Comparing results");

				// Compare the automap seed, which is rolled when the row has several cels
				MOO_CHECK_EQ(sgptDataTables->pAutomapSeed, *original_automap_seed, "Comparing automap seed");
			}
		}

		sgptDataTables->pAutoMapRand[dwAutomapLevelType] = moo_previous_rand;
		original_data_tables->pAutoMapRand[dwAutomapLevelType] = original_previous_rand;
		sgptDataTables->pAutoMap = moo_previous_automap;
		original_data_tables->pAutoMap = original_previous_automap;
		sgptDataTables->pAutomapSeed = moo_previous_seed;
		*original_automap_seed = original_previous_seed;
	}
	
	TEST_CASE_FIXTURE(NoOpFixture, "D2Common.0x6FD62E70" * doctest::skip(""))
	{
		// Set up function pointers
		const auto [sut, original] = make_function_pair(DATATBLS_FreeAutomap, dll_base + 0x00022E70);
		
		SUBCASE("")
		{
			// Call both implementations
			sut();
			original();
		}
	}
}
#endif
