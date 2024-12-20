# Compile Assertions files :

```gcc -Wall .\assert.c .\board.c .\board.h -o assert.exe ; .\assert.exe```

# Compile my game :

## On Windows : 

```gcc -Wall .\main_windows.c .\board.c .\board.h -o main.exe ; .\main.exe```

## On Linux :

```gcc -Wall .\main.c .\board.c .\board.h -o main.out ; .\main.out```


# Things actually check by assertions program :

(✅/❌)


| Type | Status |
| :---: | :---: |
| Modification des variables | ✅ |
| New Game | ✅ |
| New Special Game | ✅ |
| Copy Game | ✅ |
| Destroy Game | ✅ (WORK ONLY ON LINUX) |
| is hex | ✅ |
| uses_range | ✅ |
| current_player | ✅ |
| get content | ✅ |
| get winner | ✅ |
| move toward | ✅ |


| Bot | Statut |
| :---: | :---: |
| Base | ✅ |
| Hex | ❌ |
| Range | ❌ |
| Hex + Range | ❌ |