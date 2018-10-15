



struct OneFile {
	1: string name,
        2: string file_type,
	3: binary file_buffer,
	4: i64 file_size,
	5: string file_hsh,
}



service Photo {

  bool SendPhoto(1: OneFile onefile)

}



