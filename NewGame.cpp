#include "stdafx.h"
#include "NewGame.h"


System::Void NewGame::buttonStart_Click(System::Object^  sender, System::EventArgs^  e)
{
	parentForm->startNewGame((int)numericUpDownPlayers->Value, (int)numericUpDownPoints->Value);

	Close();
}