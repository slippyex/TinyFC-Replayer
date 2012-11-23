// History: This once was a simple mixer (it still is ;) that was
// used in a private MOD/TFMX player and was configured at run-time
// to mix up to 32 individual audio channels. In this particular
// version 16-bit and mono are moved back in. Most of that old code
// has not been touched though because it has done its job well even
// if it looks quite ugly. So, please bear with me if you find things
// in here that are not used by the FC player.

#include "LamePaula.h"
#include "filter.h"

uword MIXER_patterns;
uword MIXER_samples;
uword MIXER_voices;
ubyte MIXER_speed;
ubyte MIXER_count;
uword MIXER_minPeriod;
uword MIXER_maxPeriod;
EQSTATE eq;

static int i;
struct channel logChannel[32];

void (*mixerPlayRout)();

void* mixerFill16bitStereo( void*, udword );
void* (*mixerFillRout)( void*, udword ) = &mixerFill16bitStereo;

void mixerSetReplayingSpeed();
static const udword AMIGA_CLOCK = 3546895; //AMIGA_CLOCK_PAL;

static sword mix16[256];
static udword pcmFreq = 44100;

static udword samplesAdd;
static udword samplesPnt;
static uword samples, samplesOrg;

static udword toFill = 0;

static ubyte emptySample;


void channel::takeNextBuf()
{
    if (!isOn)
    {
        // If channel is off, take sample START parameters.
        start = paula.start;
        length = paula.length;
        length <<= 1;
        if (length == 0)  // Paula would play $FFFF words (!)
            length = 1;
        end = start+length;
    }
    
    repeatStart = paula.start;
    repeatLength = paula.length;
    repeatLength <<= 1;
    if (repeatLength == 0)  // Paula would play $FFFF words (!)
        repeatLength = 1;
    repeatEnd = repeatStart+repeatLength;
}

void channel::on()
{
    takeNextBuf();

    isOn = true;
}

void channel::updatePerVol()
{
    if (paula.period != curPeriod)
    {
        period = paula.period;  // !!!
        curPeriod = paula.period;
        if (curPeriod != 0)
        {
            stepSpeed = (AMIGA_CLOCK/pcmFreq) / curPeriod;
            stepSpeedPnt = (((AMIGA_CLOCK/pcmFreq)%curPeriod)*65536) / curPeriod;
        }
        else
        {
            stepSpeed = stepSpeedPnt = 0;
        }
    }
    
    volume = paula.volume;
    if (volume > 64)
    {
        volume = 64;
    }
}

void mixerInit() 
{

#ifdef __EQ__
	init_3band_state(&eq, 880,5000,44100);
	eq.lg = 1.0; // Boost bass by 50%
	eq.mg = 1.0; // Cut mid by 25%
	eq.hg = 1.0; // Leave high band alone 
#endif
	uword ui;
    long si;
	MIXER_voices = 8;
	uword voicesPerChannel = 4; //MIXER_voices/2; //channels;
    mixerFillRout = &mixerFill16bitStereo; 
    // Input samples: 80,81,82,...,FE,FF,00,01,02,...,7E,7F
    // Array: 00/x, 01/x, 02/x,...,7E/x,7F/x,80/x,81/x,82/x,...,FE/x/,FF/x 
/*    
    ui = 0;
    si = 0;
    while (si++ < 128)
		mix8[ui++] = (sbyte)(si/voicesPerChannel);
    
    si = -128;
    while (si++ < 0)
		mix8[ui++] = (sbyte)(si/voicesPerChannel);
*/    
    // Input samples: 80,81,82,...,FE,FF,00,01,02,...,7E,7F
    // Array: 0/x, 100/x, 200/x, ..., FF00/x
    ui = 0;
    si = 0;
    while (si < 128*256)
    {
		mix16[ui++] = (sword)(si/voicesPerChannel);
        si += 256;
    }
    si = -128*256;
    while (si < 0)
    {
		mix16[ui++] = (sword)(si/voicesPerChannel);
        si += 256;
    }

    for ( i = 0; i < 32; i++ )
    {
        logChannel[i].start = &emptySample;
        logChannel[i].end = &emptySample+1;
        logChannel[i].repeatStart = &emptySample;
        logChannel[i].repeatEnd = &emptySample+1;
        logChannel[i].length = 1;
        logChannel[i].curPeriod = 0;
        logChannel[i].stepSpeed = 0;
        logChannel[i].stepSpeedPnt = 0;
        logChannel[i].stepSpeedAddPnt = 0;
        logChannel[i].volume = 0;
        logChannel[i].off();
    }
    
    mixerSetReplayingSpeed();
}

void mixerFillBuffer( void* buffer, udword bufferLen )
{
    // Both, 16-bit and stereo samples take more memory.
    // Hence fewer samples fit into the buffer.
    bufferLen >>= 2; //bufferScale;
  
    while ( bufferLen > 0 )
    {
        if ( toFill > bufferLen )
        {
            buffer = (*mixerFillRout)(buffer, bufferLen);
            toFill -= bufferLen;
            bufferLen = 0;
        }
        else if ( toFill > 0 )
        {
            buffer = (*mixerFillRout)(buffer, toFill);
            bufferLen -= toFill;
            toFill = 0;   
        }
	
        if ( toFill == 0 )
        {
            (*mixerPlayRout)();

            register udword temp = ( samplesAdd += samplesPnt );
            samplesAdd = temp & 0xFFFF;
            toFill = samples + ( temp > 65535 );
	  
            for ( int i = 0; i < MIXER_voices; i++ )
            {
                if ( logChannel[i].period != logChannel[i].curPeriod )
                {
                    logChannel[i].curPeriod = logChannel[i].period;
                    if (logChannel[i].curPeriod != 0)
                    {
                        logChannel[i].stepSpeed = (AMIGA_CLOCK/pcmFreq) / logChannel[i].period;
                        logChannel[i].stepSpeedPnt = (((AMIGA_CLOCK/pcmFreq) % logChannel[i].period ) * 65536 ) / logChannel[i].period;
                    }
                    else
                    {
                        logChannel[i].stepSpeed = logChannel[i].stepSpeedPnt = 0;
                    }
                }
            }
        }
        
    } // while bufferLen
}

void mixerSetReplayingSpeed()
{
    samples = ( samplesOrg = pcmFreq / 50 );
    samplesPnt = (( pcmFreq % 50 ) * 65536 ) / 50;  
    samplesAdd = 0;
}
#define MIN(a,b) ((a<b)? a:b) 
#define MAX(a,b) ((a>b)? a:b)
#define saturate(x) MIN(MAX(-1.0,x),1.0)

//float bassBooster(float sample) {
//	static float selectivity, gain1, gain2, ratio, cap;
//	gain1 = 1.0 / (selectivity + 1.0);
//	cap = (sample + cap * selectivity) * gain1;
//	sample = saturate((sample + cap * ratio) * gain2);
//	return sample;
//}

void* mixerFill16bitStereo( void* buffer, udword numberOfSamples )
{
    sword* buffer16bit = (sword*)buffer;
	// left
	for ( i = 1; i < MIXER_voices; i+=2 )
    {
        buffer16bit = ((sword*)buffer)+1;
	
        for ( udword n = numberOfSamples; n > 0; n-- )
        {
            if ( i == 1 )
            {
                *buffer16bit = 0; //zero16bit;
            }
            logChannel[i].stepSpeedAddPnt += logChannel[i].stepSpeedPnt;
            logChannel[i].start += ( logChannel[i].stepSpeed + ( logChannel[i].stepSpeedAddPnt > 65535 ) );
            logChannel[i].stepSpeedAddPnt &= 65535;
            if ( logChannel[i].start < logChannel[i].end )
            {
                *buffer16bit += ( logChannel[i].volume * mix16[*logChannel[i].start] ) >> 6;
            }
            else
            {
                if ( logChannel[i].looping )
                {
                    logChannel[i].start = logChannel[i].repeatStart;
                    logChannel[i].end = logChannel[i].repeatEnd;
                    if ( logChannel[i].start < logChannel[i].end )
                    {
                        *buffer16bit += ( logChannel[i].volume * mix16[*logChannel[i].start] ) >> 6;
                    }
                }
            }
#ifdef __EQ__
			*buffer16bit = (sword)do_3band(&eq, (double)*buffer16bit);
#endif
            buffer16bit += 2;
        }
    }

	// right
    for ( i = 0; i < MIXER_voices; i+=2 )
    {
        buffer16bit = (sword*)buffer;
	
        for ( udword n = numberOfSamples; n > 0; n-- )
        {
            if ( i == 0 )
            {
                *buffer16bit = 0; //zero16bit;
            }
            logChannel[i].stepSpeedAddPnt += logChannel[i].stepSpeedPnt;
            logChannel[i].start += ( logChannel[i].stepSpeed + ( logChannel[i].stepSpeedAddPnt > 65535 ) );
            logChannel[i].stepSpeedAddPnt &= 65535;
            if ( logChannel[i].start < logChannel[i].end )
            {
                *buffer16bit += ( logChannel[i].volume * mix16[*logChannel[i].start] ) >> 6;
            }
            else
            {
                if ( logChannel[i].looping )
                {
                    logChannel[i].start = logChannel[i].repeatStart;
                    logChannel[i].end = logChannel[i].repeatEnd;
                    if ( logChannel[i].start < logChannel[i].end )
                    {
                        *buffer16bit += ( logChannel[i].volume * mix16[*logChannel[i].start] ) >> 6;
                    }
                }
            }
#ifdef __EQ__
			*buffer16bit = (sword)do_3band(&eq, (double)*buffer16bit);
#endif

            buffer16bit += 2;
        }
    }
    return(buffer16bit);
}

