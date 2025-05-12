# TextUtils

**TextUtils** est un utilitaire en ligne de commande conçu pour effectuer des opérations avancées sur de gros fichiers texte, telles que la **recherche de motifs**, la **génération aléatoire de texte**, et la **copie de motifs**, tout en utilisant le **multi-threading** et en **contrôlant la mémoire vive utilisée**.

## Fonctionnalités

- 🔍 **Recherche de motifs** dans un fichier texte à l'aide de l'algorithme KMP optimisé.
- 🧬 **Génération de fichiers texte aléatoires** de taille spécifiée.
- 📌 **Insertion aléatoire de motifs** dans un fichier texte.
- ⚙️ **Contrôle de la mémoire vive utilisée**.
- 🧵 **Exécution multi-thread** pour améliorer les performances sur les grandes données.

## Options disponibles

| Option courte | Option longue | Arguments | Description |
|---------------|----------------|-----------|-------------|
| `-c`          | `--copie`      | `[size_t s]` | Copie `s` fois le motif dans le texte à des positions aléatoires. |
| `-g`          | `--generate`   | `[string path] [size_t s]` | Génère un fichier à l'emplacement `path` avec `s` Ko de texte aléatoire. |
| `-h`          | `--help`       | *(aucun)* | Affiche l'aide. |
| `-m`          | `--memory`     | `[size_t]` | Mémoire vive maximale à utiliser (en Mo). |
| `-w`          | `--mot`        | `[string]` | Chemin du fichier contenant le motif à rechercher. |
| `-r`          | `--recherche`  | *(aucun)* | Lance la recherche du motif dans le texte. |
| `-txt`        | `--texte`      | `[string]` | Chemin du fichier contenant le texte. |
| `-t`          | `--threads`    | `[size_t]` | Nombre de threads à utiliser pour le traitement. |

## Exemples d'utilisation

```bash
# Génère un fichier de 100 Ko contenant du texte aléatoire
./textUtils -g DATA/mot.txt 100

# Génère un fichier de 1 Go de texte en utilisant 20 threads
./textUtils -g DATA/texte.txt 1000000 -t 20

# Copie 3 fois le contenu de mot.txt dans texte.txt à des positions aléatoires
./textUtils -c 3

# Lance la recherche du motif contenu dans mot.txt dans texte.txt
# en utilisant 100 Mo de mémoire vive maximum
./textUtils -r -m 100
