# Nat's Advent of Code 2024 Solutions

```text
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣷⠀⠀⢀⣀⣤⡤⠤⠤⠤⠤⣀⡀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⠟⢹⣿⣤⡶⠛⠉⠀⠀⠀⠀⠀⠀⠀⠈⠑⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⣿⠟⠁⢀⣿⣿⠋⣀⣀⣤⡤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⣿⣿⠟⠃⠀⣠⣾⣿⣿⠿⢛⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⠿⠛⢉⠔⠁⠀⠀⠼⠟⠋⠉⠀⣠⣾⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣶⡿⠟⠉⠀⠀⣠⠊⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣿⠟⠉⠀⠀⠀⠀⠀⠃⠀⠀⠀⠀⠀⠀⠀⣠⣿⣿⣷⣦⣤⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣾⡿⠋⠁⠈⠙⢿⣯⡙⠳⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣶⣿⠟⠉⠀⠀⠀⠀⠀⠀⠹⣷⡄⠈⠙⣦⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⡏⠀⠀⠀⠀⠀⠀⠀⠠⠤⣶⣶⡿⠿⠿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠈⠳⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠐⠒⠒⠲⠶⢶⣾⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣧⡀⠀⠀⠀⣀⠀⠀⠀⠀
⠀⠀⠀⠀⠒⠒⠲⠦⢤⣭⣿⣿⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⠀⠀⠙⢿⣿⣶⣶⣿⡧⠤⠀⠀⠀
⠀⠀⠀⠀⢀⣠⣴⣶⠿⠿⠿⠿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⢠⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⣠⣤⡴⠋⠀⠀⠀⣠⣾⣿⣿⣿⣷⣤⣤⣄⠀⠀
⠀⠀⢀⣴⣿⣿⣧⣤⡤⠄⠀⠀⠀⢉⣻⣷⣦⣀⡀⠀⠀⠀⠀⠀⠙⠳⢬⡁⠀⠀⠀⠀⣰⢣⠋⠀⠀⢀⣤⣾⣿⠟⠉⠠⠤⠶⢾⣿⣿⡿⠦
⠀⠀⢼⣿⣿⠿⠋⠁⠀⠀⠀⠀⠀⠒⠿⣿⣿⣿⣿⣷⣦⣄⣀⠀⠀⠀⠀⠙⣆⠀⠀⢠⠏⠁⢀⣠⣶⣿⡿⣿⠃⠀⠀⠀⠲⢾⣿⡛⠛⠿⣆
⠀⠀⣼⡿⠁⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣟⠛⣿⠿⠿⣿⣶⣦⣀⠀⠈⠀⠀⣨⣴⣾⣿⡿⠋⠁⣠⡏⠀⠀⠀⠀⠀⠀⠙⢿⣦⡀⠀
⠀⢰⣿⣧⣾⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣁⣀⣀⣀⣉⣙⣿⣷⣦⣴⣾⣿⣿⣭⣾⣧⣴⡾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣷⠀
⠀⠘⣿⣿⣿⣿⠀⠀⠀⠀⣀⣤⣤⣶⣶⣶⣤⣤⣀⠈⠉⠙⠛⠋⠉⠉⢉⣩⣿⣿⣯⣍⡉⠉⠉⠉⣀⣴⠶⠿⠿⢿⡋⠂⠀⠀⠀⠀⢸⣿⡆
⠀⠀⠈⠻⣿⣿⡄⠀⠀⠀⠀⠀⠀⣿⡏⠀⠀⠀⠈⠉⠀⠀⠀⠀⠀⠀⠙⠛⠦⢤⣤⣽⠋⠀⠀⠈⠁⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀⠀⢸⣿⡇
⠀⠀⠀⠀⠈⢻⣷⡀⠀⠀⠀⠀⠀⢹⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢧⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⡏⠀⠀⠀⠀⠀⣼⣿⠀
⠀⠀⠀⠀⠀⠀⠻⣿⣄⠀⠀⠀⠀⠀⠙⢿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣧⠀⠀⠀⠀⠀⠀⠀⢀⣿⠃⠀⠀⠀⠀⣼⣿⠃⠀
⠀⠀⠀⠀⠀⠀⠀⠙⢿⣷⣄⠀⠀⠀⠀⠈⠙⢷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⠀⠀⠀⠀⠀⠀⢀⣾⠋⠀⠀⠀⣠⣾⡿⠃⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣶⣄⠀⠀⠀⠀⠀⠙⠿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠂⠀⠀⠀⢀⣴⡿⠃⠀⠀⣠⣾⡿⠟⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⣷⣦⡀⠀⠀⠀⠀⠈⠙⠷⣦⣄⡀⠀⠀⠀⠀⠀⠀⣾⠀⠀⢀⣴⡿⠋⠀⢀⣤⣾⡿⠋⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣷⣤⡀⠀⠀⠀⠀⠀⠉⠛⠷⣶⣤⣄⣀⣠⣿⣴⣾⠟⠋⢀⣠⣶⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⠀⠀⠈⠛⢿⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠋⢁⣠⣤⣾⣿⠿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢀⣠⣶⠾⢟⣿⣿⣿⣿⣿⣿⣶⣦⣙⢿⣷⡄⠀⠀⠀⠀⠀⠀⠒⢲⣾⣿⠿⠟⣛⣉⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠴⠛⣡⣴⡿⠟⠋⠉⠉⠀⠀⠀⠀⠉⠻⣿⣿⣿⡄⠀⠀⠀⠀⠀⢠⣿⣿⣶⣾⣿⠿⠿⠷⠶⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣠⣾⢟⣩⠀⠀⠀⠀⢀⣠⡴⠶⠶⠶⠶⣾⣿⣿⡇⠀⠀⠀⠀⢀⣿⣿⠟⠉⠀⠀⠀⠀⠀⠲⢶⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣰⣿⣿⣿⠇⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣄⠀⠀⠀⠘⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠷⣌⠙⢷⡀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣿⣿⣿⡟⠀⠀⣠⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣧⣤⠴⠾⠟⠛⠒⠀⠀⠀⠀⠀⠀⠀⠘⢦⣄⡙⣷⡌⠇⠀⠀⠀⠀⠀
⠀⠀⠀⢠⣿⣿⣿⡇⢀⣾⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠜⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⡄⠀⠀⠀⠀⠀
⠀⠀⠀⢸⣿⡇⣿⡅⣾⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⣦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀⠀⠀⠀
⠀⠀⠀⣼⣿⠁⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⡿⢷⣦⣤⡀⠀⠀⠀⠀⠀⠀⣿⣿⣿⡇⠀⠀⠀⠀⠀
⠀⠀⠀⣿⣿⠀⢹⣿⣿⣿⣄⠀⠰⣄⠀⠀⠀⠀⠀⠀⠈⠲⣄⡀⠀⠀⠀⠀⠀⠀⠹⣿⣄⠈⠙⠻⣷⣤⡀⠀⠀⠀⣿⣿⣿⠃⠀⠀⠀⠀⠀
⠀⠀⢸⣿⡏⠀⠀⠻⠿⠻⣿⣦⡀⠘⢿⣦⣄⠀⠀⠀⠀⠀⠘⢿⣷⣤⣀⠀⠀⠀⠀⠹⣿⡄⠀⠀⠈⢻⣷⡀⠀⢠⣿⣿⡇⠀⠀⠀⠀⠀⠀
⠀⠀⣾⣿⠃⠀⠀⠀⠀⠀⠈⣿⣿⣶⣄⠻⣿⡿⣶⣤⣄⡀⠀⠀⠻⣿⣿⢷⣦⣀⠀⠀⣿⣷⠀⠀⠀⠀⣿⣷⣴⡿⠋⢿⣿⡄⠀⠀⠀⠀⠀
⠀⣰⣿⡏⠀⠀⠀⠀⠀⢀⣾⣿⠃⠈⠙⠛⠿⣷⣄⠉⠛⠻⢷⣦⣄⢹⣿⡄⠈⠛⢷⣤⣿⠏⠀⠀⠀⠀⣿⡿⠋⠀⠀⠈⢿⣿⡄⠀⠀⠀⠀
⢰⣿⡿⠀⠀⠀⠀⠀⠀⣾⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠿⣿⣿⠀⠀⠀⠻⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⠄⠀⠀⠀
```