

void rot_exec_SOLOREC( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	if ( rotNdx == ROT_BIGKNOB )
	{
		PhraseEditGlobalStrum( direction );
	}

}
