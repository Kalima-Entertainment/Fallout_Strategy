#include "j1App.h"
#include "AssetsManager.h"
#include "p2Log.h"

ModuleAssetsManager::ModuleAssetsManager() : j1Module()
{
	name = ("assetsManager");

	// TODO 0 (Solved): Open the following link and check it: https://icculus.org/physfs/docs/html/physfs_8h.html
	// TODO 1 (Solved): You must initialize and de-initialize the PhysFS API.

	// Initialize the PhysicsFS library
	// This must be called before any other PhysicsFS function
	// This should be called prior to any attempts to change your process's current working directory
	PHYSFS_init(nullptr);

	// We only need this when compiling in debug. In Release we don't need it.
	PHYSFS_mount(".", nullptr, 1);
}


ModuleAssetsManager::~ModuleAssetsManager()
{
	// Deinitialize the PhysicsFS library.
	// This closes any files opened via PhysicsFS, blanks the search/write paths, frees memory, and invalidates all of your file handles.
	// NOTE: This call can FAIL if there's a file open for writing that refuses to close
	PHYSFS_deinit();
}

bool ModuleAssetsManager::Awake(pugi::xml_node& config)
{
	// Determine if the PhysicsFS library is initialized, we can check it for avoid errors.
	if(PHYSFS_isInit())
		LOG("Asset Manager is succefully loaded");
	else
		LOG("Failed loading Asset Manager");

	// TODO 2 (Solved): Mount the desired ZIP with PhysFS.

	// Add an archive or directory to the search path.
	// If this is a duplicate, the entry is not added again, even though the function succeeds.
	// When you mount an archive, it is added to a virtual file system...
	// all files in all of the archives are interpolated into a single hierachical file tree.
	PHYSFS_mount("Assets.zip", nullptr, 1);

	return true;
}


bool ModuleAssetsManager::CleanUp()
{
	return false;
}

SDL_RWops* ModuleAssetsManager::Load(const char* path) const
{
	char* buffer;
	uint bytes = Load(path, &buffer);

	// TODO 5 (Solved): Check what is: https://wiki.libsdl.org/SDL_RWops
	// We will need a new method to load Music, FX and Textures from the memory.
	// Try to investigate SDL_RWops and Related Functions.

	// Read-only memory buffer for use with RWops, retruns a pointer to a new SDL_RWops structure
	SDL_RWops* ret = SDL_RWFromConstMem(buffer, bytes);

	return ret;

}


uint ModuleAssetsManager::Load(const char* path, char** buffer) const
{
	uint ret = 0;

	// TODO 3 (Solved): You want to return the number of bytes it has read from the file that we passed to this function. 
	// Maybe you want to search readBytes in the documentation, and investigate from there how to build the function.

	// The reading offset is set to the first byte of the file.
	// Returns a filehandle on success that we will need for the PHYSFS_fileLength
	PHYSFS_file* file = PHYSFS_openRead(path); 

	// Check for end-of-file state on a PhysicsFS filehandle.
	if (!PHYSFS_eof(file))
	{
		// Get total length of a file in bytes
		uint lenght = PHYSFS_fileLength(file); 
		*buffer = new char[lenght]; 

		// Read data from a PhysicsFS firehandle. Returns a number of bytes read.
		uint bytes = PHYSFS_readBytes(file, *buffer, lenght);

		if (bytes != lenght) 
		{
			LOG("%s" , path, "ERROR: %s" , PHYSFS_getLastError());
			RELEASE_ARRAY(buffer);
		}
		else
			ret = bytes; 
	}
	else
		LOG("%s", path, "ERROR: %s", PHYSFS_getLastError());


	// Close a PhysicsFS firehandle
	PHYSFS_close(file);

	return ret;
}


