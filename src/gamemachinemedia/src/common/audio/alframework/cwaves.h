// Waves.h: interface for the CWaves class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CWAVES_H_
#define _CWAVES_H_

#include <stdio.h>

namespace gm
{
	class GMMemoryStream;
}

#define MAX_NUM_WAVEID			1024

#define WAVE_SUCCEEDED(hr) (((WAVERESULT)(hr)) >= 0)

enum WAVEFILETYPE
{
	WF_EX  = 1,
	WF_EXT = 2
};

enum WAVERESULT
{
	WR_OK = 0,
	WR_INVALIDFILENAME					= - 1,
	WR_BADWAVEFILE						= - 2,
	WR_INVALIDPARAM						= - 3,
	WR_INVALIDWAVEID					= - 4,
	WR_NOTSUPPORTEDYET					= - 5,
	WR_WAVEMUSTBEMONO					= - 6,
	WR_WAVEMUSTBEWAVEFORMATPCM			= - 7,
	WR_WAVESMUSTHAVESAMEBITRESOLUTION	= - 8,
	WR_WAVESMUSTHAVESAMEFREQUENCY		= - 9,
	WR_WAVESMUSTHAVESAMEBITRATE			= -10,
	WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT	= -11,
	WR_OFFSETOUTOFDATARANGE				= -12,
	WR_FILEERROR						= -13,
	WR_OUTOFMEMORY						= -14,
	WR_INVALIDSPEAKERPOS				= -15,
	WR_INVALIDWAVEFILETYPE				= -16,
	WR_NOTWAVEFORMATEXTENSIBLEFORMAT	= -17
};

#if !defined(WAVE_FORMAT_EXTENSIBLE)
#define  WAVE_FORMAT_EXTENSIBLE                 0xFFFE
#endif // !defined(WAVE_FORMAT_EXTENSIBLE)

#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_
typedef struct tWAVEFORMATEX
{
    GMWord    wFormatTag;
    GMWord    nChannels;
    GMDWord   nSamplesPerSec;
    GMDWord   nAvgBytesPerSec;
    GMWord    nBlockAlign;
    GMWord    wBitsPerSample;
    GMWord    cbSize;
} WAVEFORMATEX;

#endif /* _WAVEFORMATEX_ */

#if !GM_MSVC
struct GUID
{
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
};
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
    WAVEFORMATEX    Format;
    union {
        GMWord wValidBitsPerSample;       /* bits of precision  */
        GMWord wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        GMWord wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    GMDWord         dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

typedef struct
{
	WAVEFILETYPE	wfType;
	WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
	char			*pData;
	unsigned long	ulDataSize;
	FILE			*pFile;
	unsigned long	ulDataOffset;
} WAVEFILEINFO, *LPWAVEFILEINFO;

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM 1

struct WAVEFORMAT
{
	GMWord  wFormatTag;
	GMWord  nChannels;
	GMDWord nSamplesPerSec;
	GMDWord nAvgBytesPerSec;
	GMWord  nBlockAlign;
};

struct PCMWAVEFORMAT
{
	WAVEFORMAT wf;
	GMWord     wBitsPerSample;
};

#endif

typedef int (GM_CDECL *PFNALGETENUMVALUE)( const char *szEnumName );
typedef int	WAVEID;

class CWaves  
{
public:
	CWaves();
	virtual ~CWaves();

	WAVERESULT LoadWaveFile(const char *szFilename, WAVEID *WaveID);
	WAVERESULT OpenWaveFile(const char *szFilename, WAVEID *WaveID);
	WAVERESULT ReadWaveData(WAVEID WaveID, void *pData, unsigned long ulDataSize, unsigned long *pulBytesWritten);
	WAVERESULT SetWaveDataOffset(WAVEID WaveID, unsigned long ulOffset);
	WAVERESULT GetWaveDataOffset(WAVEID WaveID, unsigned long *pulOffset);
	WAVERESULT GetWaveType(WAVEID WaveID, WAVEFILETYPE *pwfType);
	WAVERESULT GetWaveFormatExHeader(WAVEID WaveID, WAVEFORMATEX *pWFEX);
	WAVERESULT GetWaveFormatExtensibleHeader(WAVEID WaveID, WAVEFORMATEXTENSIBLE *pWFEXT);
	WAVERESULT GetWaveData(WAVEID WaveID, void **ppAudioData);
	WAVERESULT GetWaveSize(WAVEID WaveID, unsigned long *pulDataSize);
	WAVERESULT GetWaveFrequency(WAVEID WaveID, unsigned long *pulFrequency);
	WAVERESULT GetWaveALBufferFormat(WAVEID WaveID, PFNALGETENUMVALUE pfnGetEnumValue, unsigned long *pulFormat);
	WAVERESULT DeleteWaveFile(WAVEID WaveID);

	char *GetErrorString(WAVERESULT wr, char *szErrorString, unsigned long nSizeOfErrorString);
	bool IsWaveID(WAVEID WaveID);

private:
	WAVERESULT ParseFile(const char *szFilename, LPWAVEFILEINFO pWaveInfo);
	WAVERESULT ParseBuffer(gm::GMMemoryStream& ms, LPWAVEFILEINFO pWaveInfo);

public:
	WAVERESULT LoadWaveBuffer(const gm::GMBuffer& buffer, WAVEID *pWaveID);

private:
	LPWAVEFILEINFO	m_WaveIDs[MAX_NUM_WAVEID];
};

#endif // _CWAVES_H_
