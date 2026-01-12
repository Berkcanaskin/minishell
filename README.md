# minishell - İnteraktif Unix Shell

## 📚 Proje Açıklaması

**minishell**, bash gibi popüler bir Unix shell'in basitleştirilmiş versiyonudur. Bu proje, sistem programlaması, process yönetimi, signal handling ve parsing gibi ileri konuları kapsar.

## 🎯 İçeriği

minishell, kullanıcının komut girmesini, bu komutları parse etmesini ve çalıştırmasını sağlayan bir shell uygulamasıdır.

## 🛠️ Temel Özellikler

### 1. Command Parsing
- Komut satırını tokenize etme
- Quoted string'leri işleme
- Environment variable expansion ($VAR)

### 2. Builtins (Yerleşik Komutlar)
```bash
echo        # Metin yazdırır
cd          # Dizin değiştirir
pwd         # Çalışan dizini yazdırır
export      # Environment variable tanımlar
unset       # Variable'ı kaldırır
env         # Environment'ı gösterir
exit        # Shell'den çıkış
```

### 3. Pipe (|) ve Redirection (>, >>)
```bash
cat file.txt | grep "pattern"       # Pipe
echo "hello" > output.txt           # Redirect (overwrite)
echo "hello" >> output.txt          # Redirect (append)
cat < input.txt                     # Input redirect
```

### 4. Signal Handling
```bash
Ctrl+C   # SIGINT - Komut cancel
Ctrl+D   # EOF - Shell exit
Ctrl+\   # SIGQUIT
```

## 📖 Mimari Yapı

```
Input
  ↓
Lexer (Tokenize)
  ↓
Parser (AST oluştur)
  ↓
Expander ($ variables expand et)
  ↓
Executor (Komut çalıştır)
  ↓
Output
```

### Modüller

#### 1. Parser (`parser.c`, `parser_utils*.c`)
- Komut satırını parse eder
- Syntax hatalarını kontrol eder
- AST (Abstract Syntax Tree) oluşturur

#### 2. Expander (`expansion.c`, `expand*.c`)
- `$VAR` şeklindeki environment variable'ları genişletir
- `$?` exit status'ini döndürür
- Quoted string'leri işler

#### 3. Executor (`executor*.c`, `pipe.c`, `redirect.c`)
- Komutları çalıştırır
- Pipe'ları yönetir
- Redirection'ı işler
- Child process'ler oluşturur

#### 4. Builtins (`builtins*.c`)
- echo, cd, pwd, export, unset, env, exit komutlarını implement eder
- Sadece parent shell'de çalışır (fork edilmez)

#### 5. Environment (`env*.c`)
- Environment variable'larını yönetir
- export/unset işlemlerini gerçekleştirir

#### 6. Signals (`signals.c`)
- Ctrl+C, Ctrl+D gibi signal'ları işler
- Prompt davranışını kontrol eder

## 💡 Kullanım Örneği

```bash
$ minishell
minishell> echo "Hello World"
Hello World

minishell> cd ..
minishell> pwd
/Users/berkcan

minishell> export MY_VAR="test"
minishell> echo $MY_VAR
test

minishell> cat file.txt | grep "pattern"
(matching lines)

minishell> ls > output.txt
minishell> cat output.txt
(file listing)

minishell> exit
$
```

## 🛠️ Derleme ve Çalıştırma

```bash
cd minishell
make              # minishell binary'i oluşturur
./minishell       # Shell'i başlatır

# Derleme şeçenekleri
make clean        # Object dosyalarını sil
make fclean       # Tüm derlenmiş dosyaları sil
make re           # Yeniden derle
```

## 📚 Öğrenme Çıktıları

✅ Shell işleyişi derinlemesine anlaşıldı  
✅ Process creation (fork/exec) öğrenildi  
✅ Pipe ve redirection uygulandı  
✅ Signal handling implement edildi  
✅ Parsing ve AST oluşturma yapıldı  
✅ Environment variable yönetimi öğrenildi  
✅ Makefile ile linking öğrenildi  

## 🔧 Dosya Yapısı

```
minishell/
├── main.c                  # Entry point
├── parser.c                # Komut parsing
├── parser_utils*.c         # Parser yardımcıları
├── lexer.c                 # Tokenization
├── expander.c              # Variable expansion
├── expand_*.c              # Expansion helpers
├── executor.c              # Komut execution
├── executor_main.c         # Executor coordinator
├── builtins.c              # Built-in commands
├── builtins_utils*.c       # Builtin helpers
├── pipe.c                  # Pipe implementation
├── redirect.c              # Redirection
├── env.c                   # Environment management
├── env_utils*.c            # Env helpers
├── signals.c               # Signal handling
├── utils.c, utils2.c       # Utility functions
├── finder.c                # Path finder (which)
├── heredoc.c               # Heredoc handling
├── minishell.h             # Header dosyası
├── Makefile
└── libft/                  # libft kütüphanesi
```

## 📝 Önemli Konseptler

### Process Creation
```c
pid_t pid = fork();         // Process duplicate
if (pid == 0) {
    execve(path, args, env); // Child process'ı çalıştır
} else {
    waitpid(pid, &status, 0); // Parent'ın çocuğu beklemesi
}
```

### Pipe Yönetimi
```c
// pipe() sistem çağrısı
int pipe_fd[2];
pipe(pipe_fd);              // pipe_fd[0] = read, pipe_fd[1] = write

// dup2() ile redirection
dup2(pipe_fd[1], STDOUT_FILENO);  // stdout'ı write end'e bağla
```

### Signal Handling
```c
// SIGINT (Ctrl+C) handler
signal(SIGINT, sigint_handler);

void sigint_handler(int sig) {
    // Prompt'ı newline ile yazdır
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}
```

### Variable Expansion
```
Input:  "echo $HOME $USER"
Expand: "echo /Users/berkcan baskin"

Input:  "echo $$"
Expand: "echo 1234"  (current process ID)

Input:  "echo $?"
Expand: "echo 0"     (last exit status)
```

## 🎯 Teknik Zorluklar

### 1. Quoted String Handling
```bash
echo "Hello $USER"      # $USER expanded (double quotes)
echo 'Hello $USER'      # $USER not expanded (single quotes)
echo "Tab\there"        # Escape sequences
```

### 2. Heredoc (<<)
```bash
cat << EOF
This is a heredoc.
It ends with EOF on a line by itself.
EOF
```

### 3. Pipe Chain
```bash
cat file.txt | grep "pattern" | sort | uniq
# Her komut ayrı process'te çalışır
# Stdout→Stdin bağlantıları yapılır
```

### 4. File Descriptor Handling
```c
// 3+ FD'lerin kapatılması (leak prevention)
for (int i = 3; i < 4096; i++)
    close(i);
```

## 🚀 Advanced Features (Bonus)

- Logical operators: `&&`, `||`
- Parentheses: `()`
- Wildcard expansion: `*`, `?`
- History: Önceki komutları geri çağırma

## 📚 Norm Standartları

- 80 karaktere kadar satır uzunluğu
- Fonksiyon başına maksimum 25 satır
- Memory leaks yoktur
- Norm compliance sağlanmıştır

## 💡 Key Learning Points

1. **Process Management**: fork(), execve(), waitpid()
2. **IPC (Inter-Process Communication)**: Pipes
3. **File Descriptor Manipulation**: dup2()
4. **Signal Handling**: signal(), sigaction()
5. **String Parsing**: Tokenization, AST building
6. **Environment Management**: setenv(), getenv()

Bu proje, sistem programlaması ve Unix'in temel prensiplerinizi sağlam hale getirir.
