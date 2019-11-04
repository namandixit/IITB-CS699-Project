internal_function
Byte* fileRead (char *file_path, Size *size)
{
    SDL_RWops *rwops = rwops = SDL_RWFromFile(file_path, "rb");
    if (rwops == NULL) {
        logConsole(LOG_LEVEL_ERROR,
                   LOG_CHANNEL_FILE,
                   "Can't find file %s",
                   file_path);
        return NULL;
    }

    Size length = (Size)SDL_RWsize(rwops);
    Byte *file_data = malloc(length + 1);
    SDL_RWread(rwops, file_data, length, 1);

    if (size != NULL) {
        *size = length;
    }
    file_data[length] = '\0';

    SDL_RWclose(rwops);

    return file_data;
}
