#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_CONTENT 500

typedef enum {
    FILE_NODE,
    DIR_NODE
} NodeType;

typedef struct Node {
    char name[MAX_NAME];
    NodeType type;
    char content[MAX_CONTENT];

    struct Node* parent;
    struct Node* children;
    struct Node* next;
} Node;

Node* createNode(const char* name, NodeType type);
Node* findNode(Node* root, const char* path);
void addChild(Node* parent, Node* child);

void mkdir_cmd(Node* root, const char* path);
void touch_cmd(Node* root, const char* path);
void write_cmd(Node* root, const char* path, const char* data);
void read_cmd(Node* root, const char* path);
void ls_cmd(Node* root, const char* path);
void rm_cmd(Node* root, const char* path);
void rmdir_cmd(Node* root, const char* path);
void rename_cmd(Node* root, const char* oldPath, const char* newPath);

void splitPath(const char* path, char* parentPath, char* nodeName)
{
    char temp[200];
    strcpy(temp, path);

    char* lastSlash = strrchr(temp, '/');

    if (lastSlash == NULL)
    {
        strcpy(parentPath, "");
        strcpy(nodeName, temp);
    }
    else
    {
        *lastSlash = '\0';

        strcpy(parentPath, temp[0] ? temp : "/");
        strcpy(nodeName, lastSlash + 1);
    }
}

Node* createNode(const char* name, NodeType type)
{
    Node* node = (Node*)malloc(sizeof(Node));

    strcpy(node->name, name);
    node->type = type;

    node->content[0] = '\0';
    node->parent = NULL;
    node->children = NULL;
    node->next = NULL;

    return node;
}

Node* findNode(Node* root, const char* path)
{
    if (strcmp(path, "/") == 0)
        return root;

    char temp[200];
    strcpy(temp, path);

    char* token = strtok(temp, "/");
    Node* current = root;

    while (token && current)
    {
        Node* child = current->children;

        while (child)
        {
            if (strcmp(child->name, token) == 0)
                break;

            child = child->next;
        }

        if (!child)
            return NULL;

        current = child;
        token = strtok(NULL, "/");
    }

    return current;
}

void addChild(Node* parent, Node* child)
{
    child->parent = parent;

    if (!parent->children)
    {
        parent->children = child;
    }
    else
    {
        Node* temp = parent->children;

        while (temp->next)
            temp = temp->next;

        temp->next = child;
    }
}

void mkdir_cmd(Node* root, const char* path)
{
    char parentPath[200];
    char nodeName[50];

    splitPath(path, parentPath, nodeName);

    Node* parent = findNode(root, parentPath);

    if (!parent || parent->type != DIR_NODE)
    {
        printf("Invalid path: %s\n", parentPath);
        return;
    }

    Node* existing = parent->children;

    while (existing)
    {
        if (strcmp(existing->name, nodeName) == 0)
        {
            printf("Directory already exists: %s\n", nodeName);
            return;
        }

        existing = existing->next;
    }

    Node* newDir = createNode(nodeName, DIR_NODE);
    addChild(parent, newDir);

    printf("Directory '%s' created.\n", path);
}

void touch_cmd(Node* root, const char* path)
{
    char parentPath[200];
    char nodeName[50];

    splitPath(path, parentPath, nodeName);

    Node* parent = findNode(root, parentPath);

    if (!parent || parent->type != DIR_NODE)
    {
        printf("Invalid path: %s\n", parentPath);
        return;
    }

    Node* existing = parent->children;

    while (existing)
    {
        if (strcmp(existing->name, nodeName) == 0)
        {
            printf("File already exists: %s\n", nodeName);
            return;
        }

        existing = existing->next;
    }

    Node* newFile = createNode(nodeName, FILE_NODE);
    addChild(parent, newFile);

    printf("File '%s' created.\n", path);
}

void write_cmd(Node* root, const char* path, const char* data)
{
    Node* file = findNode(root, path);

    if (!file || file->type != FILE_NODE)
    {
        printf("File not found: %s\n", path);
        return;
    }

    strcpy(file->content, data);

    printf("Written to file '%s'.\n", path);
}

void read_cmd(Node* root, const char* path)
{
    Node* file = findNode(root, path);

    if (!file || file->type != FILE_NODE)
    {
        printf("File not found: %s\n", path);
        return;
    }

    printf("Reading file '%s':\n%s\n", path, file->content);
}

void ls_cmd(Node* root, const char* path)
{
    Node* dir = findNode(root, path);

    if (!dir || dir->type != DIR_NODE)
    {
        printf("Directory not found: %s\n", path);
        return;
    }

    Node* child = dir->children;

    while (child)
    {
        printf("%s\t%s\n",
               child->name,
               (child->type == DIR_NODE ? "<DIR>" : "<FILE>"));

        child = child->next;
    }
}

void rm_cmd(Node* root, const char* path)
{
    Node* file = findNode(root, path);

    if (!file || file->type != FILE_NODE)
    {
        printf("File not found: %s\n", path);
        return;
    }

    Node* parent = file->parent;

    if (parent->children == file)
    {
        parent->children = file->next;
    }
    else
    {
        Node* temp = parent->children;

        while (temp->next != file)
            temp = temp->next;

        temp->next = file->next;
    }

    free(file);

    printf("File '%s' deleted.\n", path);
}

void rmdir_cmd(Node* root, const char* path)
{
    Node* dir = findNode(root, path);

    if (!dir || dir->type != DIR_NODE)
    {
        printf("Directory not found: %s\n", path);
        return;
    }

    if (dir->children)
    {
        printf("Directory not empty: %s\n", path);
        return;
    }

    Node* parent = dir->parent;

    if (parent->children == dir)
    {
        parent->children = dir->next;
    }
    else
    {
        Node* temp = parent->children;

        while (temp->next != dir)
            temp = temp->next;

        temp->next = dir->next;
    }

    free(dir);

    printf("Directory '%s' deleted.\n", path);
}

void rename_cmd(Node* root, const char* oldPath, const char* newPath)
{
    Node* node = findNode(root, oldPath);

    if (!node)
    {
        printf("File/Directory not found: %s\n", oldPath);
        return;
    }

    char parentPath[200];
    char nodeName[50];

    splitPath(newPath, parentPath, nodeName);

    Node* newParent = findNode(root, parentPath);

    if (!newParent || newParent->type != DIR_NODE)
    {
        printf("Invalid path: %s\n", parentPath);
        return;
    }

    Node* oldParent = node->parent;

    if (oldParent->children == node)
    {
        oldParent->children = node->next;
    }
    else
    {
        Node* temp = oldParent->children;

        while (temp->next != node)
            temp = temp->next;

        temp->next = node->next;
    }

    node->next = NULL;

    strcpy(node->name, nodeName);

    addChild(newParent, node);

    printf("Renamed/Moved to '%s'.\n", newPath);
}

int main()
{
    Node* root = createNode("/", DIR_NODE);

    char command[500];

    printf("=== File System Simulation ===\n");

    while (1)
    {
        printf(">> ");

        fgets(command, sizeof(command), stdin);

        command[strcspn(command, "\n")] = '\0';

        if (strncmp(command, "mkdir ", 6) == 0)
        {
            mkdir_cmd(root, command + 6);
        }
        else if (strncmp(command, "touch ", 6) == 0)
        {
            touch_cmd(root, command + 6);
        }
        else if (strncmp(command, "write ", 6) == 0)
        {
            char* path = strtok(command + 6, " ");
            char* data = strtok(NULL, "");

            if (path && data)
                write_cmd(root, path, data);
        }
        else if (strncmp(command, "read ", 5) == 0)
        {
            read_cmd(root, command + 5);
        }
        else if (strncmp(command, "ls ", 3) == 0)
        {
            ls_cmd(root, command + 3);
        }
        else if (strncmp(command, "rm ", 3) == 0)
        {
            rm_cmd(root, command + 3);
        }
        else if (strncmp(command, "rmdir ", 6) == 0)
        {
            rmdir_cmd(root, command + 6);
        }
        else if (strncmp(command, "rename ", 7) == 0)
        {
            char* oldPath = strtok(command + 7, " ");
            char* newPath = strtok(NULL, "");

            if (oldPath && newPath)
                rename_cmd(root, oldPath, newPath);
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command!\n");
        }
    }

    return 0;
}