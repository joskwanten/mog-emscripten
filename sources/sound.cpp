#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_sound.h"
#include "sound.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "MOGtypes.h"

#define AUDIO_BUFFER	4096


/* Paths: */ 
extern char *default_g_path;
extern char *default_s_path;
extern char **g_paths;
extern int n_g_paths,act_g_path;
extern char *g_path;
extern char **s_paths;
extern int n_s_paths,act_s_path;
extern char *s_path;
extern int music_volume;

bool sound_enabled=false;

Mix_Music * music = NULL;

int music_position=0;
bool playing_music=false;
bool music_loaded[8]={false,false,false,false,false,false,false,false};			
Sound_Sample *music_sound[8]={0,0,0,0,0,0,0,0};
char music_files[8][128];		/* File names */ 
char music_realfiles[8][128];	/* File names including music directory */ 


bool Sound_initialization(void)
{
	SDL_Init(SDL_INIT_AUDIO);
  	int ret = Mix_OpenAudio(0, 0, 0, 0); // we ignore all these..
	if (ret != 0) {
		printf("Failed to initialize sound.\n");
	}
	printf("Successful initialized sound system.\n");
    // char SoundcardName[256];
	// int audio_rate = 44100;
	// int audio_channels = 2;
	// int audio_bufsize = AUDIO_BUFFER;
	// Uint16 audio_format = AUDIO_S16;
	// SDL_version compile_version;

	sound_enabled=true;
	// fprintf (stderr, "Initializing SDL_mixer.\n");
	// if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_bufsize))  {
	//   fprintf (stderr, "Unable to open audio: %s\n", Mix_GetError());
	//   sound_enabled=false;
	//   fprintf (stderr, "Running the game without audio.\n");
	//   return false;	
	// } /* if */ 

	// SDL_AudioDriverName (SoundcardName, sizeof (SoundcardName));
	// //Mix_QuerySpec (&audio_rate, &audio_format, &audio_channels);
	// fprintf (stderr, "    opened %s at %d Hz %d bit %s, %d bytes audio buffer\n",
	// 		 SoundcardName, audio_rate, audio_format & 0xFF,
	// 		 audio_channels > 1 ? "stereo" : "mono", audio_bufsize);
	// MIX_VERSION (&compile_version);
	// fprintf (stderr, "    compiled with SDL_mixer version: %d.%d.%d\n",
	// 		 compile_version.major,
	// 		 compile_version.minor,
	// 		 compile_version.patch);
	// // fprintf (stderr, "    running with SDL_mixer version: %d.%d.%d\n",
	// // 		 Mix_Linked_Version()->major,
	// // 		 Mix_Linked_Version()->minor,
	// // 		 Mix_Linked_Version()->patch);

	// Sound_Init();
	//Mix_HookMusic(myMusicPlayer, 0);

	return true;
} /* Sound_init */ 

void Sound_release(void)
{
	// if (sound_enabled) {
	// 	Sound_Quit();
	// 	Mix_CloseAudio();
	// } /* if */ 
	// sound_enabled=false;
} /* Sound_release */ 


/* a check to see if file is readable and greater than zero */
int file_check(char *fname)
{
	FILE *fp;

	if ((fp=fopen(fname,"r"))!=NULL) {
		if (fseek(fp,0L, SEEK_END)==0 && ftell(fp)>0) {
  			fclose(fp);
			return true;
		} /* if */ 
		/* either the file could not be read (==-1) or size was zero (==0) */ 
		fprintf(stderr,"ERROR in file_check(): the file %s is corrupted.\n", fname);
		fclose(fp);
		exit(1);
	} /* if */ 
	return false;
} /* file_check */ 



SOUNDT Sound_create_sound(char *file,int flags)
{
	int n_ext=6;
	char *ext[6]={".WAV",".OGG",".MP3",".wav",".ogg",".mp3"};
	char name[256],name2[256];
	int i;

	//printf("Loading %s\n", file);

	if (sound_enabled) {
		for(i=0;i<n_ext;i++) {
			strcpy(name,file);
			strcat(name,ext[i]);
			sprintf(name2,"%s%s",s_path,name);
			if (file_check(name2)) return Mix_LoadWAV(name2);
		} /* for */ 
		for(i=0;i<n_ext;i++) {
			strcpy(name,file);
			strcat(name,ext[i]);
			sprintf(name2,"%s%s",default_s_path,name);
			if (file_check(name2)) return Mix_LoadWAV(name2);
		} /* for */ 

		fprintf(stderr,"ERROR in Sound_create_sound(): Could not load sound file: %s%s.(wav|ogg|mp3)\n",s_path, file);
		exit(1);
	} else {
		return 0;
	} /* if */ 
} /* Sound_create_sound */ 


void Delete_sound(SOUNDT s)
{
	//if (sound_enabled) Mix_FreeChunk(s);
} /* Delete_sound */ 


void Sound_play(SOUNDT s)
{

	if (sound_enabled) {
		int res = Mix_PlayChannel(-1,s,0);	
		//printf("Played sound with result %d\n", res);
	}
} /* Sound_play */ 


void Sound_obtain_file_name(char *file,char *fullname)
{
	int n_ext=6;
	char *ext[6]={".WAV",".OGG",".MP3",".wav",".ogg",".mp3"};
	char name[256],name2[256];
	int i;

	for(i=0;i<n_ext;i++) {
		strcpy(name,file);
		strcat(name,ext[i]);
		sprintf(name2,"%s%s",s_path,name);
		if (file_check(name2)) {
			strcpy(fullname,name2);
			return;
		} /* if */ 
		strcpy(name,file);
		strcat(name,ext[i]);
		sprintf(name2,"%s%s",default_s_path,name);
		if (file_check(name2)) {
			strcpy(fullname,name2);
			return;
		} /* if */ 

	} /* for */ 

	fprintf(stderr,"ERROR in Sound_obtain_file_name(): Could not find sound file: %s%s.(wav|ogg|mp3)\n",s_path, file);
 	fullname[0]=0;
	exit(1);
} /* Sound_obtain_file_name */ 


Sound_Sample *Sound_create_stream(char *file)
{
	int n_ext=6;
	char *ext[6]={".WAV",".OGG",".MP3",".wav",".ogg",".mp3"};
	char name[256],name2[256];
	int i;

	Sound_AudioInfo inf;

	inf.format=AUDIO_S16;
	inf.channels=2;
	inf.rate=44100;

	if (sound_enabled) {
		for(i=0;i<n_ext;i++) {
			strcpy(name,file);
			strcat(name,ext[i]);
			sprintf(name2,"%s%s",s_path,name);
			if (file_check(name2)) return Sound_NewSampleFromFile(name2,&inf,AUDIO_BUFFER);
			if (file_check(name2)) return Sound_NewSampleFromFile(name2,0,AUDIO_BUFFER);
		} /* for */ 
		for(i=0;i<n_ext;i++) {
			strcpy(name,file);
			strcat(name,ext[i]);
			sprintf(name2,"%s%s",default_s_path,name);
			if (file_check(name2)) return Sound_NewSampleFromFile(name2,&inf,AUDIO_BUFFER);
			if (file_check(name2)) return Sound_NewSampleFromFile(name2,0,AUDIO_BUFFER);
		} /* for */ 
		
	 	fprintf(stderr,"ERROR in Sound_create_stream(): Could not load sound file: %s%s.(wav|ogg|mp3)\n",s_path, file);
		exit(1);
	} else {
		return 0;
	} /* if */ 
} /* Sound_create_stream */ 

// Mix_Chunk jkwSound_create_stream(char *fileName) {
// 	struct stat info;
// 	int result = stat(filename, &info);
// 	char * bytes = malloc( info.st_size );
// 	FILE * f = fopen( "noise.ogg", "rb" );
// 	fread( bytes, 1, info.st_size, f  );
// 	fclose(f);

// 	SDL_RWops * ops = SDL_RWFromConstMem(bytes, info.st_size);
// 	Mix_Chunk sound3 = Mix_LoadWAV_RW(ops, 0);
// 	SDL_FreeRW(ops);
// 	free(bytes);

// 	return sound3;
// } 

char musicToPlay[3][256];
int leftToPlay = 0;
int playIndex = 0;
char *loopmusic;

void playSong(char *f1);

void musicFinished() {
	//printf("Music finished %d, %d!\n", leftToPlay, playIndex);
	if (leftToPlay > 0) {
		leftToPlay--;
		playSong(musicToPlay[++playIndex]);
	} else if(playing_music) {
		playSong(musicToPlay[playIndex]);
	}
}

void musicFinishedLoop() {
	//printf("Music finished %d, %d!\n", leftToPlay, playIndex);	

	if (loopmusic) {
		playSong(loopmusic);	
	}
}

void playSong(char *f1) {
	char tmp[256];
	printf("playSong %s\n", f1);
	Sound_obtain_file_name(f1,tmp);
	
	if (music) 
	{
	  	Mix_FreeMusic(music);
	  	music = NULL;
	}
	
	music = Mix_LoadMUS(tmp);	

	if (strcmp(f1, "castle") == 0 || strcmp(f1, "world") == 0 || strcmp(f1, "story") == 0) {
		loopmusic = f1;
		Mix_HookMusicFinished(musicFinishedLoop);
	} else {
		Mix_HookMusicFinished(musicFinished);
	}

    Mix_PlayMusic(music, 0);
}

void Sound_create_music(char *f1,char *f2,char *f3)
{
	//printf("Sound_create_music called with %s, %s, %s\n", f1, f2, f3);
	leftToPlay = 0;
	loopmusic = NULL;

	if (f2) 
	{
		strcpy(musicToPlay[1], f2);		
		leftToPlay++;
	}
	
	if (f3) {
		strcpy(musicToPlay[2], f3);				
		leftToPlay++;
	}

	if (f1) 
	{
		strcpy(musicToPlay[0], f1); 		
		playIndex = 0;
		playSong(f1);
	}

	// SOUNDT mymusic = Sound_create_sound(f1, 0);
	// Sound_play(mymusic);
	// Mix_FreeChunk(mymusic);

	// char tmp[128];
	// int seq_len=0;

	// if (sound_enabled) {
	// 	if (f1!=0) {
	// 		Sound_obtain_file_name(f1,tmp);
	// 		strcpy(music_files[0],f1);
	// 		strcpy(music_realfiles[0],tmp);
	// 		music_loaded[0]=true;
	// 		music_sound[0]=jkwSound_create_stream(f1);
	// 		seq_len=1;
	// 	} else {
	// 		music_files[0][0]=0;
	// 		music_loaded[0]=false;
	// 	} /* if */ 
	// 	if (f2!=0) {
	// 		Sound_obtain_file_name(f2,tmp);
	// 		strcpy(music_files[1],f2);
	// 		strcpy(music_realfiles[1],tmp);
	// 		music_loaded[1]=true;
	// 		music_sound[1]=Sound_create_stream(f2);
	// 		seq_len=2;
	// 	} else {
	// 		music_files[1][0]=0;
	// 		music_loaded[1]=false;
	// 	} /* if */ 
	// 	if (f3!=0) {
	// 		Sound_obtain_file_name(f3,tmp);
	// 		strcpy(music_files[2],f3);
	// 		strcpy(music_realfiles[2],tmp);
	// 		music_loaded[2]=true;
	// 		music_sound[2]=Sound_create_stream(f3);
	// 		seq_len=3;
	// 	} else {
	// 		music_files[2][0]=0;
	// 		music_loaded[2]=false;
	// 	} /* if */ 

	// 	music_position=0;
	// 	playing_music=true;
	// } /* if */ 
} /* Sound_create_music */ 

void Sound_subst_music(char *f)
{	
// 	char tmp[128];

// 	Sound_obtain_file_name(f,tmp);
// //	my_SubstMusic(tmp);
} /* Sound_sust_music */ 


void Sound_subst_music_now(char *f)
{
	// Sound_release_music();
	// Sound_create_music(f,0,0);
} /* Sound_sust_music_now */ 



void Sound_release_music(void)
{
	loopmusic = NULL;
	Mix_HaltMusic();
	Mix_FreeMusic(music);	

	// int i;

	// if (sound_enabled) {
	// 	playing_music=false;
	// 	for(i=0;i<3;i++) {
	// 		if (music_loaded[i]) Sound_FreeSample(music_sound[i]);
	// 		music_loaded[i]=false;
	// 	} /* if */ 
	// } /* if */ 
} /* Sound_release_music */ 


void Sound_temporary_release_music(void)
{
	// int i;

	// if (sound_enabled) {
	// 	playing_music=false;
	// 	for(i=0;i<3;i++) {
	// 		if (music_loaded[i]) Sound_FreeSample(music_sound[i]);
	// 	} /* if */ 
	// } /* if */ 
} /* Sound_release_music */ 


void Sound_pause_music(void)
{
	Mix_PauseMusic();
	playing_music=false;	
} /* Sound_pause_music */ 


void Sound_unpause_music(void)
{
	Mix_ResumeMusic();
	playing_music=true;
} /* Sound_unpause_music */ 


void music_recovery(void)
{	
// 	char tmp[128];
// 	int seq_len=0;
// //	FILE *fp;

// 	if (sound_enabled) {
// //		fp=fopen("report.txt","w");
// //		fprintf(fp,"%s\n%s\n%s\n",music_files[0],music_files[1],music_files[2]);
// //		fclose(fp);

// 		if (music_loaded[0]) { 
// 			music_loaded[0]=true;
// 			Sound_obtain_file_name(music_files[0],tmp);
// 			strcpy(music_realfiles[0],tmp);
// 			music_sound[0]=jkwSound_create_stream(music_files[0]);
// 			seq_len=1;
// 		} /* if */ 
// 		if (music_loaded[1]) { 
// 			music_loaded[1]=true;
// 			Sound_obtain_file_name(music_files[1],tmp);
// 			strcpy(music_realfiles[1],tmp);
// 			music_sound[1]=jkwSound_create_stream(music_files[1]);
// 			seq_len=2;
// 		} /* if */ 
// 		if (music_loaded[2]) { 
// 			music_loaded[2]=true;
// 			Sound_obtain_file_name(music_files[2],tmp);
// 			strcpy(music_realfiles[2],tmp);
// 			music_sound[2]=jkwSound_create_stream(music_files[2]);
// 			seq_len=3;
// 		} /* if */ 

// 		playing_music=true;
// 	} /* if */ 

} /* music_recovery */ 


void myMusicPlayer(void *udata, Uint8 *stream, int len)
{
	printf("myMusicPlayer called!\n");
	// int i,act=0;
	// Sint16 *ptr2;

	// if (stream!=0) {
	// 	ptr2=(Sint16 *)stream;
	// 	if (playing_music) {
	// 		while(act<len) {
	// 			if (music_loaded[music_position]) {
	// 				/* Play a music file: */ 

	// 				if ((music_sound[music_position]->flags&SOUND_SAMPLEFLAG_EOF)) {
	// 					/* End of file: */ 
	// 					if (music_loaded[music_position+1]) {
	// 						music_position++;
	// 					} /* if */ 
	// 					// TODO: Sound_Rewind(music_sound[music_position]);
	// 				} else {
	// 					/* In the middle of the file: */ 
	// 					int decoded=0;
	// 					Sint16 *ptr;

	// 					// TODO: Sound_SetBufferSize(music_sound[music_position], len-act);
						
	// 					//TODO: decoded=Sound_Decode(music_sound[music_position]);
	// 					ptr=(Sint16 *)music_sound[music_position]->buffer;
	// 					for(i=0;i<decoded;i+=2,ptr++,ptr2++) {
	// 						*ptr2=((Sint32(*ptr)*Sint32(music_volume))/127);
	// 					} /* for */ 
	// 					act+=decoded;
	// 				} /* if */ 
	// 			} else {
	// 				/* No music file loaded: */ 
	// 				for(i=act;i<len;i++) stream[i]=0;
	// 				act=len;
	// 			} /* if */  
	// 		} /* while */ 
	// 	} else {
	// 		/* No music to play: */ 
	// 		for(i=0;i<len;i++) stream[i]=0;
	// 	} /* if */ 
	// } else {
	// 	fprintf(stderr,"ERROR in myMusicPlayer(): null music stream!!\n");
	// } /* if */ 
} /* myMusicPlayer */ 










