#include "Music.h"

Music* Music::instance = nullptr;

Music::Music()
{
	f_masterVolume = 2.f;
}

Music::~Music()
{
	engine->drop();
}
/******************************************************************************/
/*!
\brief
reading from text file and making a vector of links
\param link
link to the textfile to read from
\return
a vector of string that contains all the link to music files
*/
/******************************************************************************/
vector<string> Music::readFromText(string link)
{
	ifstream inData;
	inData.open(link, std::ifstream::in);
	vector<string> vec_dialog;

	if (!inData)
	{
		std::cout << "Error Opening" << link << std::endl;
		exit(1);
	}
	if (inData.good())
	{
		while (inData.good())
		{
			string data;
			std::getline(inData, data);
			vec_dialog.push_back(data);
		}
	}
	inData.close();
	return vec_dialog;
}
/******************************************************************************/
/*!
\brief
intialise music
*/
/******************************************************************************/
void Music::Init()
{
	engine = irrklang::createIrrKlangDevice();
	musicVec = readFromText("Music//musicList.txt");


	int i = 0;
	for (vector<string>::iterator it = musicVec.begin(); it != musicVec.end(); ++it)
	{
		sound.push_back(engine->addSoundSourceFromFile((*it).c_str()) );
		sound[i]->setDefaultVolume(1.f);
		++i;
	}

}
/******************************************************************************/
/*!
\brief
playing the sound source
\param index
index of the sound source to be played
\param loop
whether to loop the sound file or not
*/
/******************************************************************************/
void Music::PlayMusic(int index,bool loop,float volume)
{
	sound[index]->setDefaultVolume(volume * f_masterVolume);
	engine->play2D(sound[index], loop,false);
}

void Music::StopAllSound()
{
	engine->stopAllSounds();
}

Music* Music::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Music();
	}
	return instance;
}