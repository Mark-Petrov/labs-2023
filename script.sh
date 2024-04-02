function traverse_directory {
    local dir="$1"
    for file in "$dir"/*; do
        if [[ -d "$file" ]]; then
            echo "$file"  # тута полный путь поддиректории
            traverse_directory "$file"  # Рекурсивно обходим поддиректорию
        fi
    done
}

read -p "Введите путь к директории: " directory

if [[ -d "$directory" ]]; then
    echo "Поддиректории в каталоге $directory:"
    traverse_directory "$directory"
else
    echo "Каталог $directory не существует или не является директорией."
fi
