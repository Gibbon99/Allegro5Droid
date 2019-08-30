#include <hdr/io/io_logFile.h>
#include <bitset>
#include <hdr/io/io_fileSystem.h>
#include "hdr/io/io_resourceLevel.h"

#define MAP_VERSION                115

std::unordered_map<std::string, _levelStruct> shipLevel;
cpVect drawOffset;

//---------------------------------------------------------
//
// Convert current tile information into padded array
// Suitable for display on current screen size
void lvl_addPaddingToLevel (const std::string fileName)
//---------------------------------------------------------
{
	std::vector<int> tempLevel;
	cpVect           tempDimensions{};
	int              countY, countX, whichTile;
	int              destX, destY;
	std::string      levelName;

	drawOffset.x = (screenWidth / TILE_SIZE);
	drawOffset.y = (screenHeight / TILE_SIZE);

	destX = drawOffset.x / 2;
	destY = drawOffset.y / 2;

	tempDimensions.x = shipLevel.at (fileName).levelDimensions.x;
	tempDimensions.y = shipLevel.at (fileName).levelDimensions.y;

	printf("Level [ %s ] dimensions [ %f %f ]\n", fileName.c_str(), shipLevel.at (fileName).levelDimensions.x, shipLevel.at (fileName).levelDimensions.y);

	tempLevel.reserve ((shipLevel.at (fileName).levelDimensions.x + drawOffset.x) *
	                   (shipLevel.at (fileName).levelDimensions.y + drawOffset.y));

	tempLevel.assign ((shipLevel.at (fileName).levelDimensions.x + drawOffset.x) *
	                  (shipLevel.at (fileName).levelDimensions.y + drawOffset.y), 0);

	for (countY = 0; countY != shipLevel.at (fileName).levelDimensions.y; countY++)
		{
			destX       = (drawOffset.x / 2);
			for (countX = 0; countX != shipLevel.at (fileName).levelDimensions.x; countX++)
				{
					whichTile = shipLevel.at (fileName).tiles[(countY * shipLevel.at (fileName).levelDimensions.x) + countX];
					tempLevel[(destY * (tempDimensions.x + drawOffset.x)) + destX] = whichTile;
					destX++;
				}
			destY++;
		}

	tempDimensions.x += drawOffset.x;
	tempDimensions.y += drawOffset.y;

	shipLevel.at (fileName).levelDimensions.x = tempDimensions.x;
	shipLevel.at (fileName).levelDimensions.y = tempDimensions.y;

	shipLevel.at (fileName).tiles.clear ();

	shipLevel.at (fileName).tiles.reserve (tempDimensions.x * tempDimensions.y);

	shipLevel.at (fileName).tiles.assign (tempDimensions.x * tempDimensions.y, 0);

	for (int i = 0; i != tempDimensions.x * tempDimensions.y; i++)
		{
			shipLevel.at (fileName).tiles[i] = tempLevel[i];
		}
	tempLevel.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Populate the shipdeck structure from a file in memory
bool lvl_loadShipLevel (const std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_MEM_FILE *fp;
	int           checkVersion;
	_levelStruct  tempLevel = _levelStruct ();
	_lineSegment  tempSegment;
	cpVect        tempWaypoint;
	int           tempDroidType;
	int           tempTile;
	int  fileSize = 0;
	char *memoryBuffer;

	drawOffset.x = (screenWidth); // Padding to make tilePosX always positive
	drawOffset.y = (screenHeight); // Padding to make tilePosY always positive

	fileSize     = io_getFileSize(fileName.c_str());
	if (fileSize < 0)
		log_logMessage(LOG_LEVEL_INFO, sys_getString("Fatal error getting level file size [ %s ].", fileName.c_str()));

	memoryBuffer = (char *) malloc(sizeof(char) * fileSize);
	if (nullptr == memoryBuffer)
		{
			log_logMessage(LOG_LEVEL_INFO, sys_getString("Fatal memory allocation error when loading level file."));
		}

	io_getFileIntoMemory(fileName.c_str(), memoryBuffer);


	//
	// Open the block of memory and read like a file
	fp = para_openMemFile (memoryBuffer, fileSize);
	if (nullptr == fp)
		{
			log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Mapping memory to file failed for file [ %s ]", fileName.c_str ()));
		}
	//
	// Check this version is ok to use
	para_readFile (fp, &checkVersion, sizeof (checkVersion));
	if (checkVersion != MAP_VERSION)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("MAP_VERSION wrong for file [ %s ]", fileName.c_str ()));
			log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Wanted [ %i ] got from file [ %i ]", MAP_VERSION, checkVersion));
		}
	//
	// Read number variables
	para_readFile (fp, (void *) &tempLevel.numLineSegments, sizeof (tempLevel.numLineSegments));
	para_readFile (fp, (void *) &tempLevel.numWaypoints, sizeof (tempLevel.numWaypoints));
	para_readFile (fp, (void *) &tempLevel.numDroids, sizeof (tempLevel.numDroids));
	para_readFile (fp, (void *) &tempLevel.numLifts, sizeof (tempLevel.numLifts));
	para_readFile (fp, (void *) &tempLevel.levelDimensions, sizeof (tempLevel.levelDimensions));

	//
	// Line segments for physics collisions
	for (int i = 0; i != tempLevel.numLineSegments; i++)
		{
			para_readFile (fp, &tempSegment.start, sizeof (tempSegment.start));
			para_readFile (fp, &tempSegment.finish, sizeof (tempSegment.finish));

			tempSegment.start.x += drawOffset.x;
			tempSegment.start.y += drawOffset.y;

			tempSegment.finish.x += drawOffset.x;
			tempSegment.finish.y += drawOffset.y;

			tempLevel.lineSegments.push_back (tempSegment);
		}
	//
	// Waypoints for Droid patrol
	for (int i = 0; i != tempLevel.numWaypoints; i++)
		{
			para_readFile (fp, &tempWaypoint, sizeof (tempWaypoint));
			//
			// Why is this needed?  Pixel offset from screen res?
			//
			tempWaypoint.x += drawOffset.x;
			tempWaypoint.y += drawOffset.y;

			tempLevel.wayPoints.push_back (tempWaypoint);
		}
	//
	// Load each droid type on the current level
	for (int i = 0; i != tempLevel.numDroids; i++)
		{
			para_readFile (fp, &tempDroidType, sizeof (tempDroidType));
			tempLevel.droidTypes.push_back (tempDroidType);
		}
	//
	// Array holding tile types
	for (int i = 0; i != tempLevel.levelDimensions.x * tempLevel.levelDimensions.y; i++)
		{
			para_readFile (fp, &tempTile, sizeof (tempTile));
			tempLevel.tiles.push_back (tempTile);
		}
	para_readFile (fp, &tempLevel.levelName, sizeof (tempLevel.levelName));

	//
	// Finished - close the file
	para_closeFile (fp);

	//
	// Extract the deck number from the filename
	std::string output     = fileName;
	std::string removePart = "newDeck";

	output.erase (0, removePart.size ());
	std::string::size_type idx = output.rfind ('.');

	if (idx != std::string::npos)
		{
			output.erase (idx, 4);
			tempLevel.deckNumber = std::stoi (output, nullptr, 10);
		}
	else
		{
			tempLevel.deckNumber = -1;
			log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to parse deck filename for index [ %s ]", fileName.c_str ()));
		}

	std::bitset<32> testMask;

	testMask.reset ();
	testMask[tempLevel.deckNumber] = true;

	tempLevel.deckCategory = static_cast<cpBitmask>(testMask.to_ulong ());

	// Generate physics masks and categories
	tempLevel.wallPhysicsCreated  = false;
	tempLevel.droidPhysicsCreated = false;

	shipLevel.insert (std::pair<std::string, _levelStruct> (tempLevel.levelName, tempLevel));

	lvl_addPaddingToLevel (tempLevel.levelName);

	log_logMessage (LOG_LEVEL_INFO, sys_getString ("Loaded ship level [ %s ] - index [ %i ]", tempLevel.levelName, tempLevel.deckNumber));

	return true;
}


//-----------------------------------------------------------------------------------------------------
//
// Return the levelName from the passed in deckNumber
std::string lvl_returnLevelNameFromDeck (int deckNumber)
//-----------------------------------------------------------------------------------------------------
{
//
// Using   for (auto levelItr : levelInfo)
// Results in massive FPS drop
//
	for (auto const &levelItr : shipLevel)
		{
			if (deckNumber == levelItr.second.deckNumber)
				{
					return levelItr.second.levelName;
				}
		}
	return "Not Found";
}

//-----------------------------------------------------------------------------------------
//
// Return the deckNumber for the passed in level string
int lvl_getDeckNumber (const std::string levelName)
//-----------------------------------------------------------------------------------------
{
	auto levelItr = shipLevel.find (levelName);

	if (levelItr != shipLevel.end ())
		{
			if (levelItr->second.mapVersion == -1)  // Not loaded properly
				{
					log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Trying to access invalid level [ %s ]", levelName.c_str ()));
					return -1;
				}
			return levelItr->second.deckNumber;
		}
	log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Unable to find levelName [ %s ] - [ %s ]", levelName.c_str (), __func__));

	return -1;   // Not found
}

//--------------------------------------------------------------------------
//
// Render the waypoint segments
void lvl_showWayPoints (const std::string levelName)
//--------------------------------------------------------------------------
{
	int indexCount;
	cpVect lineStart;
	cpVect lineFinish;
	_lineSegment tempLine;
	cpVect wallStartDraw, wallFinishDraw;

	indexCount = 0;

	for (auto it: shipLevel.at (levelName).wayPoints)
		{
			tempLine.start = shipLevel.at (levelName).wayPoints[indexCount];

			if (indexCount + 1 < shipLevel.at (levelName).numWaypoints)
				tempLine.finish = shipLevel.at (levelName).wayPoints[indexCount + 1];
			else
				tempLine.finish = shipLevel.at (levelName).wayPoints[0];

			lineStart.x = static_cast<float>(tempLine.start.x);
			lineStart.y = static_cast<float>(tempLine.start.y);

			lineFinish.x = static_cast<float>(tempLine.finish.x);
			lineFinish.y = static_cast<float>(tempLine.finish.y);

			wallStartDraw = sys_worldToScreen (lineStart, 50);
			wallFinishDraw = sys_worldToScreen (lineFinish, 50);

			al_draw_line (wallStartDraw.x, wallStartDraw.y, wallFinishDraw.x, wallFinishDraw.y, al_map_rgb (255, 100, 255), 2);

			indexCount++;
		}
}
