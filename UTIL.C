/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char* lexeme)
{
    switch (token)
    {
    case IF:
    case ELSE:
    case INT:
    case RETURN:
    case VOID:
    case WHILE:
        fprintf(listing,"reserved word \" %s\"\n",lexeme);
        break;
    case PLUS:		fprintf(listing,"+"); break;
    case MINUS:	    fprintf(listing,"-"); break;
    case TIMES:	    fprintf(listing,"*"); break;
    case DIVIDE:	fprintf(listing,"/"); break;
    case LT:		fprintf(listing,"<"); break;
    case GT:		fprintf(listing,">"); break;
    case ASSIGN:	fprintf(listing,"="); break;
    case NEQ:		fprintf(listing,"!="); break;
    case SEMI:		fprintf(listing,";\n"); break;
    case COMMA:     fprintf(listing,",\n"); break;
    case LPAREN:	fprintf(listing,"(\n"); break;
    case RPAREN:	fprintf(listing,")\n"); break;
    case LBRACE:	fprintf(listing,"{\n"); break;
    case RBRACE:	fprintf(listing,"}\n"); break;
    case LSQUARE:	fprintf(listing,"[\n"); break;
    case RSQUARE:	fprintf(listing,"]\n"); break;
    case LTE:		fprintf(listing,"<=" ); break;
    case GTE:		fprintf(listing,">="); break;
    case EQ:		fprintf(listing,"=="); break;
    case NUM:
        fprintf(listing,"NUM, value = %s\n",lexeme);
        break;
    case ID:
        fprintf(listing,"ID, name = \" %s\"\n",lexeme);
        break;
    case ENDOFFILE:
        fprintf(listing,"EOF\n");
        break;
    case ERROR:
        fprintf(listing,"<<<ERROR >>> %s\n",lexeme);
        break;
    default:
        fprintf(listing," <<<UNKNOWN TOKEN >>> %d\n",token);
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
  }
  return t;
}

TreeNode* newDecNode(DecKind kind)
{
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DecK;
        t->kind.dec = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = (char*)malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=4
#define UNINDENT indentno-=4

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

char* typeName(ExpType type)
{
    static char i[] = "integer";
    static char v[] = "void";
    static char invalid[] = "<<invalid type>>";

    switch (type)
    {
    case Integer: return i; break;
    case Void:    return v; break;
    default:      return invalid;
    }
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind == DecK)
    {
        switch (tree->kind.dec)
        {
        case ScalarDecK:
            fprintf(listing, "[Scalar declaration \"%s\" of type \"%s\"]\n"
                , tree->name, typeName(tree->variableDataType));
            break;
        case ArrayDecK:
            fprintf(listing, "[Array declaration \"%s\" of size %d"
                " and type \"%s\"]\n",
                tree->name, tree->val, typeName(tree->variableDataType));
            break;
        case FuncDecK:
            fprintf(listing, "[Function declaration \"%s()\""
                " of return type \"%s\"]\n",
                tree->name, typeName(tree->functionReturnType));
            break;
        default:
            fprintf(listing, "<<<unknown declaration type>>>\n");
            break;
        }
    }
    else if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
    case CompoundK:
        fprintf(listing, "[Compound statement]\n");
        break;
    case IfK:
        fprintf(listing, "[IF statement]\n");
        break;
    case WhileK:
        fprintf(listing, "[WHILE statement]\n");
        break;
    case ReturnK:
        fprintf(listing, "[RETURN statement]\n");
        break;
    case CallK:
        fprintf(listing, "[Call to function \"%s()\"]\n",
            tree->name);
        break;
    default:
        fprintf(listing, "<<<unknown statement type>>>\n");
        break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
    case OpK:
        fprintf(listing, "[Operator \"");
        printToken(tree->op, "");
        fprintf(listing, "\"]\n");
        break;
    case IdK:
        fprintf(listing, "[Identifier \"%s", tree->name);
        fprintf(listing, "\"]\n");
        break;
    case ConstK:
        fprintf(listing, "[Literal constant \"%d\"]\n", tree->val);
        break;
    case AssignK:
        fprintf(listing, "[Assignment]\n");
        break;
    default:
        fprintf(listing, "<<<unknown expression type>>>\n");
        break;
      }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
