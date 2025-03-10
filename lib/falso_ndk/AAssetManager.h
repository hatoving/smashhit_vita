#ifndef ANDROID_ASSET_MANAGER_H
#define ANDROID_ASSET_MANAGER_H

#include <sys/cdefs.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AAssetManager;
/**
 * {@link AAssetManager} provides access to an application's raw assets by
 * creating {@link AAsset} objects.
 *
 * AAssetManager is a wrapper to the low-level native implementation
 * of the java {@link AAssetManager}, a pointer can be obtained using
 * AAssetManager_fromJava().
 *
 * The asset hierarchy may be examined like a filesystem, using
 * {@link AAssetDir} objects to peruse a single directory.
 *
 * A native {@link AAssetManager} pointer may be shared across multiple threads.
 */
typedef struct AAssetManager AAssetManager;

struct AAssetDir;
/**
 * {@link AAssetDir} provides access to a chunk of the asset hierarchy as if
 * it were a single directory. The contents are populated by the
 * {@link AAssetManager}.
 *
 * The list of files will be sorted in ascending order by ASCII value.
 */
typedef struct AAssetDir AAssetDir;

struct AAsset;
/**
 * {@link AAsset} provides access to a read-only asset.
 *
 * {@link AAsset} objects are NOT thread-safe, and should not be shared across
 * threads.
 */
typedef struct AAsset AAsset;

/** Available access modes for opening assets with {@link AAssetManager_open} */
enum {
    /** No specific information about how data will be accessed. **/
    AASSET_MODE_UNKNOWN      = 0,
    /** Read chunks, and seek forward and backward. */
    AASSET_MODE_RANDOM       = 1,
    /** Read sequentially, with an occasional forward seek. */
    AASSET_MODE_STREAMING    = 2,
    /** Caller plans to ask for a read-only buffer with all data. */
    AASSET_MODE_BUFFER       = 3
};

/**
 * [Non-Standard]: Create new AAssetManager object
 */

AAssetManager * AAssetManager_create();

/**
 * Open an asset.
 *
 * The object returned here should be freed by calling AAsset_close().
 */
AAsset* AAssetManager_open(AAssetManager* mgr, const char* filename, int mode);

/**
 * Close the asset, freeing all associated resources.
 */
void AAsset_close(AAsset* asset);

/**
 * Attempt to read 'count' bytes of data from the current offset.
 *
 * Returns the number of bytes read, zero on EOF, or < 0 on error.
 */
int AAsset_read(AAsset* asset, void* buf, size_t count);

/**
 * Seek to the specified offset within the asset data.  'whence' uses the
 * same constants as lseek()/fseek().
 *
 * Returns the new position on success, or (off_t) -1 on error.
 */
off_t AAsset_seek(AAsset* asset, off_t offset, int whence);

/**
 * Open a new file descriptor that can be used to read the asset data. If the
 * start or length cannot be represented by a 32-bit number, it will be
 * truncated. If the file is large, use AAsset_openFileDescriptor64 instead.
 *
 * Returns < 0 if direct fd access is not possible (for example, if the asset is
 * compressed).
 */
int AAsset_openFileDescriptor(AAsset* asset, off_t* outStart, off_t* outLength);


#ifdef __cplusplus
};
#endif

#endif      // ANDROID_ASSET_MANAGER_H