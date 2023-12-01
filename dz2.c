#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define E 2.71828

//DEFINICIJE TIPOVA
typedef struct ListNode {
	double info;
	struct ListNode *next;
} ListNode;

typedef struct StackNode {
	struct TreeNode* info;
	struct StackNode *next;
} StackNode;

typedef struct TreeNode {
	char info;
	int flag;
	struct TreeNode *left, *right;
} TreeNode;

//GET NODE ZA STABLO
struct TreeNode* GetTreeNode() {
	struct TreeNode* p;
	p = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->flag = 0;
	p->left = NULL;
	p->right = NULL;
	return p;
}

//F-JE ZA STEK NA KOJI SE STAVLJAJU CHAR-ovi
void PushC(struct ListNode** stack, double x) {
	struct ListNode* p;
	p = (struct ListNode*)malloc(sizeof(struct ListNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->info = x;
	p->next = *stack;
	*stack = p;
	return;
}

double PopC(struct ListNode** stack) {
	struct ListNode* p;
	double x;
	if (*stack == NULL) {
		printf("Underflow\n");
		exit(1);
	}
	else {
		p = *stack;
		*stack = p->next;
		x = p->info;
		free(p);
		return x;
	}
}

int StackEmptyC(struct ListNode** stack) {
	if (*stack == NULL) return 1;
	else return 0;
}


//F-JE ZA STEK NA KOJI SE STAVLJAJU CVOROVI STABLA
void PushN(struct StackNode** stack, struct TreeNode* x) {
	struct StackNode* p;
	p = (struct StackNode*)malloc(sizeof(struct StackNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->info = x;
	p->next = *stack;
	*stack = p;
	return;
}

struct TreeNode* PopN(struct StackNode** stack) {
	struct StackNode* p;
	struct TreeNode* x;
	if (*stack == NULL) {
		printf("Underflow\n");
		exit(1);
	}
	else {
		p = *stack;
		*stack = p->next;
		x = p->info;
		free(p);
		return x;
	}
}

int StackEmptyN(struct StackNode** stack) {
	if (*stack == NULL) return 1;
	else return 0;
}


//F_JE ZA RED
void Insert(struct StackNode** front, struct StackNode** rear, struct TreeNode* x) {
	struct StackNode* p;
	p = (struct StackNode*)malloc(sizeof(struct StackNode));
	if (p == NULL) {
		printf("Neuspesna alokacija\n");
		exit(1);
	}
	p->info = x;
	p->next = NULL;
	if (*rear == NULL)
		*front = p;
	else
		(*rear)->next = p;
	*rear = p;
	return;
}

struct TreeNode* Delete(struct StackNode** front, struct StackNode** rear) {
	struct StackNode* p;
	struct TreeNode* x;
	if (*front == NULL) {
		printf("Underflow\n");
		exit(1);
	}
	else {
		p = *front;
		x = p->info;
		*front = p->next;
		if (*front == NULL)
			*rear = NULL;
		free(p);
		return x;
	}
}


//F-JE ZA FORMIRANJE I STAMPANJE STABLA I POSTFIKSA I IZRACUNAVANJE IZRAZA I NJEGOVOG IZVODA
char* InputExpression() {
	int i, num, numFlag;
	char c, *exp = NULL, *p;
	printf("\nNapomena: dozvoljeno koristiti celobrojne konstante i promenljive oznacene bilo kojim velikim slovom abecede osim slovom E koje je rezervisano "
		"za konstantu E = 2.71828\n\n");
	printf("Unesite izraz:\n");
	exp = malloc(sizeof(char));
	i = 1; num = 0;
	numFlag = 0;
	p = exp;
	while ((c = getchar()) != '\n') {
		if (c >= '0' && c <= '9') {
			num = num * 10 + (c - '0');
			numFlag = 1;
			continue;
		}
		if (numFlag) {
			if (num > 128) {
				printf("Program ne podrzava konstante vece od 128\n");
				//treba da pukne
			}
			*p = (char)(-num);
			exp = realloc(exp, (i + 1) * sizeof(char));
			if (exp == NULL) {
				printf("Neuspesna alokacija\n");
				exit(1);
			}
			p++; i++;
			numFlag = 0; num = 0;
		}
		if (c == 'l') {
			getchar(); getchar();
			continue;
		}
		if (c == ',') {
			i = i + 3;
			exp = realloc(exp, i * sizeof(char));
			if (exp == NULL) {
				printf("Neuspesna alokacija\n");
				exit(1);
			}
			*p = ')';
			*(p + 1) = 'l';
			*(p + 2) = '(';
			p = p + 3;
			continue;
		}
		*p = c;
		exp = realloc(exp, (i + 1) * sizeof(char));
		if (exp == NULL) {
			printf("Neuspesna alokacija\n");
			exit(1);
		}
		p++; i++;
		if (c == 'e') {
			getchar(); getchar();
			continue;
		}
		if (c == 'a') {
			getchar(); getchar();
			continue;
		}
		if (*(p - 1) == '-' && *(p - 2) == '(')
			*(p - 1) = '~';
	}
	if (numFlag) {
		*p = (char)(-num);
		exp = realloc(exp, (i + 1) * sizeof(char));
		if (exp == NULL) {
			printf("Neuspesna alokacija\n");
			exit(1);
		}
		p++; i++;
	}
	*p = '\0';
	return exp;
}

int SprStackTop(struct ListNode** stack, int* spr, char* op) {
	if (StackEmptyC(stack)) return -1;
	char topStack = (*stack)->info;
	for (int i = 0; i < 10; i++) {
		if (topStack == op[i])
			return spr[i];
	}
}

struct TreeNode* InitTree(char* exp) {
	int i, currentIPR, currentSPR;
	char *p, x;
	char op[] = { '+', '-', '*', '/', '^', '~', 'l', 'e', 'a', '(', ')' };
	int ipr[] = { 2, 2, 3, 3, 5, 6, 8, 8, 8, 9, 1 }, spr[] = { 2, 2, 3, 3, 4, 6, 7, 7, 7, 0 };
	struct ListNode* stackC = NULL;
	struct StackNode* stackN = NULL;
	struct TreeNode* r;
	p = exp;
	while (*p != '\0') {
		if ((*p >= 'A' && *p <= 'Z') || *p < 0) {
			r = GetTreeNode();
			r->info = *p;
			PushN(&stackN, r);
		}
		else {
			for (i = 0; i < 11; i++) {
				if (*p == op[i]) {
					currentIPR = ipr[i];
					currentSPR = spr[i];
					break;
				}
			}
			while (!(StackEmptyC(&stackC)) && currentIPR <= SprStackTop(&stackC, spr, op)) {
				x = PopC(&stackC);
				if (x == '~' || x == 'e' || x == 'a') {
					r = GetTreeNode();
					r->info = x;
					r->left = PopN(&stackN);
					PushN(&stackN, r);
				}
				else {
					r = GetTreeNode();
					r->info = x;
					r->right = PopN(&stackN);
					r->left = PopN(&stackN);
					PushN(&stackN, r);
				}
			}
			if (*p != ')')
				PushC(&stackC, *p);
			else
				x = PopC(&stackC);
		}
		p++;
	}
	while (!(StackEmptyC(&stackC))) {
		x = PopC(&stackC);
		if (x == '~' || x == 'e' || x == 'a') {
			r = GetTreeNode();
			r->info = x;
			r->left = PopN(&stackN);
			PushN(&stackN, r);
		}
		else {
			r = GetTreeNode();
			r->info = x;
			r->right = PopN(&stackN);
			r->left = PopN(&stackN);
			PushN(&stackN, r);
		}
	}
	return PopN(&stackN);
}

void Postfix(struct TreeNode* root) {
	if (root == NULL) {
		printf("0\n");
		return;
	}
	int i = 0;
	char *exp = NULL, *q;
	struct StackNode* stack = NULL;
	struct TreeNode *p = root;
	while (p != NULL) {
		PushN(&stack, p);
		p = p->left;
	}
	while (!(StackEmptyN(&stack))) {
		p = PopN(&stack);
		if (p->flag == 0) {
			p->flag = 1;
			PushN(&stack, p);
			p = p->right;
			while (p != NULL) {
				PushN(&stack, p);
				p = p->left;
			}
		}
		else {
		p->flag = 0;
		exp = realloc(exp, (i + 1) * sizeof(char));
		*(exp + i) = p->info;
		i++;
		}
	}
	exp = realloc(exp, (i + 1) * sizeof(char));
	*(exp + i) = '\0';
	for (q = exp; *q != '\0'; q++) {
		if (*q < 0) {
			printf("%d", (int)(-(*q)));
		}
		else {
			switch (*q) {
			case 'l':
				printf("log");
				break;
			case 'e':
				printf("exp");
				break;
			case 'a':
				printf("abs");
				break;
			default:
				printf("%c", *q);
			}
		}
	}
	printf("\n");
	return;
}

double Calculate(struct TreeNode* root, double* var) {
	if (root == NULL) return 0;
	double x, y, z;
	struct TreeNode *p = root;
	struct ListNode* stackC = NULL;
	struct StackNode* stackN = NULL;
	while (p != NULL) {
		PushN(&stackN, p);
		p = p->left;
	}
	while (!(StackEmptyN(&stackN))) {
		p = PopN(&stackN);
		if (p->flag == 0) {
			p->flag = 1;
			PushN(&stackN, p);
			p = p->right;
			while (p != NULL) {
				PushN(&stackN, p);
				p = p->left;
			}
		}
		else {
			p->flag = 0;
			if ((p->info >= 'A' && p->info <= 'Z') || p->info < 0) {
				if (p->info > 0)
					PushC(&stackC, var[p->info - 'A']);
				else
					PushC(&stackC, (double)(-p->info));
			}
			else {
				if (p->info == '~' || p->info == 'e' || p->info == 'a') {
					x = PopC(&stackC);
					switch (p->info) {
					case '~':
						z = -x;
						break;
					case 'e':
						z = pow(E, x);
						break;
					case 'a':
						if (x < 0) z = -x;
						else z = x;
						break;
					}
				}
				else {
					y = PopC(&stackC);
					x = PopC(&stackC);
					switch (p->info) {
					case '+':
						z = x + y;
						break;
					case '-':
						z = x - y;
						break;
					case '*':
						z = x * y;
						break;
					case '/':
						if (y == 0) {
							printf("Greska: Deljenje sa nulom\n");
							exit(1);
						}
						z = x / y;
						break;
					case '^':
						if (x < 0 && y < 1 && (int)(1 / y) % 2 == 0){
							printf("Greska: Korenovanje negativne vrednosti\n");
							exit(1);
						}					
						z = pow(x, y);
						break;
					case 'l':
						if (x == 1) {
							printf("Greska: Baza logaritma ne sme biti 1\n");
							exit(1);
						}
						if (x < 0 || y < 0) {
							printf("Greska: Argumenti funkcije log moraju biti pozitivni\n");
							exit(1);
						}
						z = log(y) / log(x);
					}
				}
				PushC(&stackC, z);
			}
		}
	}
	return PopC(&stackC);
}

int TreeHeight(struct TreeNode* root) {
	if (root == NULL)
		return 0;
	int i, height, queueSize;
	struct TreeNode* p = NULL;
	struct StackNode* front = NULL;
	struct StackNode* rear = NULL;
	Insert(&front, &rear, root);
	queueSize = 1;
	height = 0;
	while ((front != NULL) && (rear != NULL)) {
		for (i = queueSize; i > 0; i--) {
			p = Delete(&front, &rear);
			queueSize--;
			if (p->left != NULL) {
				Insert(&front, &rear, p->left);
				queueSize++;
			}
			if (p->right != NULL) {
				Insert(&front, &rear, p->right);
				queueSize++;
			}
		}
		height++;
	}
	return height;
}

void PrintSpaces(int n) {
	for (int i = 0; i < n; i++)
		printf(" ");
	return;
}

void PrintTree(struct TreeNode* root) {
	if (root == NULL) {
		printf("0\n");
		return;
	}
	int height, level, nextLevelFlag;
	struct TreeNode *p, *q;
	struct StackNode* front = NULL;
	struct StackNode* rear = NULL;
	height = TreeHeight(root);
	Insert(&front, &rear, root);
	Insert(&front, &rear, NULL);
	level = 0; nextLevelFlag = 0;
	while (level < height) {
		p = Delete(&front, &rear);
		if (p == NULL) {
			level++;
			nextLevelFlag = 0;
			printf("\n");
			if ((front != NULL) && (rear != NULL)) 
				Insert(&front, &rear, NULL);
			continue;
		}
		else {
			if (nextLevelFlag == 0)
				PrintSpaces(pow(2, (height - level - 1)) - 1);
			else
				PrintSpaces(pow(2, (height - level)) - 1);
			nextLevelFlag = 1;
			if (p->info < 0)
				printf("%d", (int)(-(p->info)));
			else 
				printf("%c", p->info);
		}
		if (p->left != NULL)
			Insert(&front, &rear, p->left);
		else {
			q = GetTreeNode();
			q->info = ' ';
			Insert(&front, &rear, q);
		}
		if (p->right != NULL)
			Insert(&front, &rear, p->right);
		else {
			q = GetTreeNode();
			q->info = ' ';
			Insert(&front, &rear, q);
		}
	}
	while ((front == NULL) && (rear == NULL))
		Delete(&front, &rear);
	return;
}

struct TreeNode* CopyTree(struct TreeNode* root) {
	if (root == NULL) return NULL;
	int i = 0;
	struct StackNode *stack = NULL, *stackN = NULL;
	struct TreeNode *p = root, *q = NULL;
	while (p != NULL) {
		PushN(&stack, p);
		p = p->left;
	}
	while (!(StackEmptyN(&stack))) {
		p = PopN(&stack);
		if (p->flag == 0) {
			p->flag = 1;
			PushN(&stack, p);
			p = p->right;
			while (p != NULL) {
				PushN(&stack, p);
				p = p->left;
			}
		}
		else {
			p->flag = 0;
			if ((p->info >= 'A' && p->info <= 'Z') || p->info < 0) {
				q = GetTreeNode();
				q->info = p->info;
				PushN(&stackN, q);
			}
			else {
				if (p->info == '~' || p->info == 'e' || p->info == 'a') {
					q = GetTreeNode();
					q->info = p->info;
					q->left = PopN(&stackN);
				}
				else {
					q = GetTreeNode();
					q->info = p->info;
					q->right = PopN(&stackN);
					q->left = PopN(&stackN);
				}
				PushN(&stackN, q);
			}
		}
	}
	return q;
}

struct TreeNode* Derivate(struct TreeNode* tree, char var) {
	if (tree == NULL) return NULL;
	struct StackNode* stack = NULL; struct StackNode* stackD = NULL;
	struct TreeNode *derivateTree, *p, *q, *x, *y, *z;
	derivateTree = CopyTree(tree);
	p = derivateTree;
	q = tree;
	while (p != NULL) {
		PushN(&stackD, p);
		p = p->left;
		PushN(&stack, q);
		q = q->left;
	}
	while (!(StackEmptyN(&stackD))) {
		p = PopN(&stackD);
		q = PopN(&stack);
		if (p->flag == 0) {
			p->flag = 1;
			PushN(&stackD, p);
			p = p->right;
			PushN(&stack, q);
			q = q->right;
			while (p != NULL) {
				PushN(&stackD, p);
				p = p->left;
				PushN(&stack, q);
				q = q->left;
			}
		}
		else {
			p->flag = 0;
			if ((p->info >= 'A' && p->info <= 'Z') || p->info < 0) {
				if (p->info == var)
					p->info = (char)(-1);
				else
					p->info = 0;
			}
			else {
				switch (p->info) {
				case '+': {
					if (p->left->info == 0 && p->right->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						free(p->right); p->right = NULL;
						break;
					}
					if (p->left->info == 0) {
						free(p->left);
						x = p->right;
						p->info = x->info;
						p->left = x->left;
						p->right = x->right;
						free(x);
					}
					else {
						if (p->right->info == 0) {
							free(p->right);
							x = p->left;
							p->info = x->info;
							p->left = x->left;
							p->right = x->right;
							free(x);
						}
					}
					break;
				}
				case '-': {
					if (p->left->info == 0 && p->right->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						free(p->right); p->right = NULL;
						break;
					}
					if (p->right->info == 0) {
						free(p->right);
						x = p->left;
						p->info = x->info;
						p->left = x->left;
						p->right = x->right;
						free(x);
					}
					else {
						if (p->left->info == 0) {
							p->info = '~';
							free(p->left);
							p->left = p->right;
							p->right = NULL;
						}
					}
					break;
				}
				case '*': {
					if (p->left->info == 0 && p->right->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						free(p->right); p->right = NULL;
						break;
					}
					x = GetTreeNode();
					x->info = '*';
					x->left = p->left;
					x->right = CopyTree(q->right);
					if (x->left->info == 0) {
						x = GetTreeNode();
						x->info = 0;
					}
					else {
						if ((int)(-(x->left->info)) == 1) {
							free(x->left);
							y = x->right;
							x->info = y->info;
							x->left = y->left;
							x->right = y->right;
						}
					}
					p->left = x;
					y = GetTreeNode();
					y->info = '*';
					y->left = CopyTree(q->left);
					y->right = p->right;
					if (y->right->info == 0) {
						y = GetTreeNode();
						y->info = 0;
					}
					else {
						if ((int)(-(y->right->info)) == 1) {
							free(y->right);
							z = y->left;
							y->info = z->info;
							y->left = z->left;
							y->right = z->right;
						}
					}
					p->right = y;
					p->info = '+';
					if (p->left->info == 0 && p->right->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						free(p->right); p->right = NULL;
					}
					if (p->left->info == 0) {
						free(p->left);
						x = p->right;
						p->info = x->info;
						p->left = x->left;
						p->right = x->right;
						free(x);
					}
					else {
						if (p->right->info == 0) {
							free(p->right);
							x = p->left;
							p->info = x->info;
							p->left = x->left;
							p->right = x->right;
							free(x);
						}
					}
					break;
				}
				case '/': {
					if (p->left->info == 0 && p->right->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						free(p->right); p->right = NULL;
						break;
					}
					x = GetTreeNode();
					y = GetTreeNode();
					y->info = '*';
					y->left = p->left;
					y->right = CopyTree(q->right);
					if (y->left->info == 0) {
						y = GetTreeNode();
						y->info = 0;
					}
					else {
						if ((int)(-(y->left->info)) == 1) {
							free(y->left);
							z = y->right;
							y->info = z->info;
							y->left = z->left;
							y->right = z->right;
						}
					}
					x->left = y;
					z = GetTreeNode();
					z->info = '*';
					z->left = CopyTree(q->left);
					z->right = p->right;
					if (z->right->info == 0) {
						z = GetTreeNode();
						z->info = 0;
					}
					else {
						if ((int)(-(z->right->info)) == 1) {
							free(z->right);
							y = z->left;
							z->info = y->info;
							z->left = y->left;
							z->right = y->right;
						}
					}
					x->right = z;
					x->info = '-';
					if (x->left->info == 0 && x->right->info == 0) {
						x->info = 0;
						free(x->left); x->left = NULL;
						free(x->right); x->right = NULL;
					}
					else {
						if (x->right->info == 0) {
								free(x->right);
								y = x->left;
								x->info = y->info;
								x->left = y->left;
								x->right = y->right;
								free(y);
							}
						else {
							if (x->left->info == 0) {
								x->info = '~';
								free(x->left);
								x->left = x->right;
								x->right = NULL;
							}
						}
					}
					p->left = x;
					x = GetTreeNode();
					x->info = '^';
					x->left = CopyTree(q->right);
					y = GetTreeNode();
					y->info = (char)(-2);
					x->right = y;
					p->right = x;
					break;
				}
				case '^': {
					if (p->left->info == 0 && p->right->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						free(p->right); p->right = NULL;
						break;
					}
					if (p->left->info == 0) {
						x = GetTreeNode();
						x->info = '^';
						x->left = CopyTree(q->left);
						x->right = CopyTree(q->right);
						z = GetTreeNode();
						z->info = 'l';
						y = GetTreeNode();
						y->info = 'E';
						z->left = y;
						z->right = CopyTree(q->left);
						if ((int)(-(p->right->info)) == 1) {
							p->info = '*';
							free(p->left); p->left = x;
							free(p->right); p->right = z;
						}
						else {
							y = GetTreeNode();
							y->info = '*';
							y->left = x;
							y->right = z;
							p->info = '*';
							free(p->left); p->left = y;
						}
						break;
					}
					if (p->right->info == 0) {
						if (q->right->info < 0) {
							x = GetTreeNode();
							x->info = q->right->info + 1;
							if (q->right->info + 1 == 0) {
								free(x);
								y = GetTreeNode();
								y->info = (char)(-1);
							}
							else {
								if ((int)(-(q->right->info + 1)) == 1) {
									y = GetTreeNode();
									y = CopyTree(q->left);
								}
								else {
									y = GetTreeNode();
									y->info = '^';
									y->left = CopyTree(q->left);
									y->right = x;
								}
							}
						}
						else {
							x = GetTreeNode();
							x->info = '-';
							x->left = CopyTree(q->right);
							z = GetTreeNode();
							z->info = (char)(-1);
							x->right = z;
							y = GetTreeNode();
							y->info = '^';
							y->left = CopyTree(q->left);
							y->right = x;
						}
						if ((int)(-(p->left->info)) == 1) {
							if ((int)(-(y->info)) == 1) {
								p->info = CopyTree(q->right);
							}
							else {
								p->info = '*';
								free(p->left); p->left = CopyTree(q->right);
								free(p->right); p->right = y;
							}
						}
						else {
							if ((int)(-(y->info)) == 1) {
								z = GetTreeNode();
								z = CopyTree(q->right);
								p->info = '*';
								free(p->right);
								p->right = p->left;
								p->left = z;
							}
							else {
								z = GetTreeNode();
								z->info = '*';
								z->left = CopyTree(q->right);
								z->right = y;
								p->info = '*';
								free(p->right);
								p->right = p->left;
								p->left = z;
							}
						}
						break;
					}
					if (p->left->info != 0 && p->right->info != 0) {
						printf("Program ne podrzava izracunavanje izrada koje sadze logaritamsko diferenciranje");
						exit(1);
						break;
					}
					break;
				}
				case '~': {
					if (p->left->info == 0) {
						free(p->left);
						p->info = 0;
						p->left = NULL;
					}
					break;
				}
				case 'l': {
					z = GetTreeNode();
					z->info = '-';
					if (p->right->info == 0) {
						y = GetTreeNode();
						y->info = 0;
						z->left = y;
					}
					else {
						x = GetTreeNode();
						if (q->left->info == 'E') {
							x->info = '/';
							x->left = p->right;
							x->right = CopyTree(q->right);
							z->left = x;
						}
						else {
							x->info = 'l';
							x->right = CopyTree(q->left);
							y = GetTreeNode();
							y->info = 'E';
							x->left = y;
							y = GetTreeNode();
							y->info = '*';
							y->left = x;
							x = GetTreeNode();
							x->info = '/';
							x->left = p->right;
							x->right = CopyTree(q->right);
							y->right = x;
							z->left = y;
						}
					}
					if (p->left->info == 0) {
						y = GetTreeNode();
						y->info = 0;
						z->right = y;
					}
					else {
						x = GetTreeNode();
						if (q->right->info == 'E') {
							x->info = '/';
							x->left = p->left;
							x->right = CopyTree(q->left);
							z->right = x;
						}
						else {
							x->info = 'l';
							x->right = CopyTree(q->right);
							y = GetTreeNode();
							y->info = 'E';
							x->left = y;
							y = GetTreeNode();
							y->info = '*';
							y->left = x;
							x = GetTreeNode();
							x->info = '/';
							x->left = p->left;
							x->right = CopyTree(q->left);
							y->right = x;
							z->right = y;
						}
					}
					if (z->left->info == 0 && z->right->info == 0) {
						p->info = 0;
						free(z); 
						p->left = NULL; p->right = NULL;
						break;
					}
					if (z->right->info == 0) {
						free(z->right);
						x = z->left;
						z->info = x->info;
						z->left = x->left;
						z->right = x->right;
						free(x);
					}
					else {
						if (z->left->info == 0) {
							z->info = '~';
							free(z->left);
							z->left = z->right;
							z->right = NULL;
						}
					}
					if (q->left->info == 'E') {
						p->info = z->info;
						p->left = z->left;
						p->right = z->right;
						free(z);
					}
					else {
						p->info = '/';
						p->left = z;
						z = GetTreeNode();
						z->info = '^';
						x = GetTreeNode();
						x->info = 'l';
						x->right = CopyTree(q->left);
						y = GetTreeNode();
						y->info = 'E';
						x->left = y;
						z->left = x;
						y = GetTreeNode();
						y->info = (char)(-2);
						z->right = y;
						p->right = z;
					}
					break;
				}
				case 'e': {
					if (p->left->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						break;
					}
					if ((int)(-(p->left->info)) == 1) {
						p->info = 'e';
						p->left = CopyTree(q->left);
					}
					else {
						x = GetTreeNode();
						x->info = 'e';
						x->left = CopyTree(q->left);
						p->info = '*';
						p->right = p->left;
						p->left = x;
					}
					break;
				}
				case 'a': {
					if (p->left->info == 0) {
						p->info = 0;
						free(p->left); p->left = NULL;
						break;
					}
					if ((int)(-(p->left->info)) == 1) {
						x = GetTreeNode();
						x->info = 'a';
						x->left = CopyTree(q->left);
						p->info = '/';
						p->left = CopyTree(q->left);
						p->right = x;
					}
					else {
						x = GetTreeNode();
						x->info = 'a';
						x->left = CopyTree(q->left);
						y = GetTreeNode();
						y->info = '/';
						y->left = CopyTree(q->left);
						y->right = x;
						p->info = '*';
						p->right = p->left;
						p->left = y;
					}
					break;
				}
				}
			}
		}
	}
	if (derivateTree->info == 0) {
		return NULL;
	}
	return derivateTree;
}

void DeallocateTree(struct TreeNode* root) {
	if (root == NULL) return;
	int i = 0;
	struct StackNode* stack = NULL;
	struct TreeNode *p = root;
	while (p != NULL) {
		PushN(&stack, p);
		p = p->left;
	}
	while (!(StackEmptyN(&stack))) {
		p = PopN(&stack);
		if (p->flag == 0) {
			p->flag = 1;
			PushN(&stack, p);
			p = p->right;
			while (p != NULL) {
				PushN(&stack, p);
				p = p->left;
			}
		}
		else {
			p->flag = 0;
			free(p);
		}
	}
	return;
}


int main() {
	int flag = 1;
	double res, variables[26] = {0};
	char u, *exp = NULL, *p, var;
	struct TreeNode *tree = NULL, *derivateTree = NULL;
	printf("Meni:\n1. Unos izraza\n2. Ispis stabla\n3. Ispis izraza u postfiksnoj notaciji\n4. Zadavanje vrednosti promenljivih\n5. Racunanje izraza\n"
		"6. Diferenciranje izraza po zadatoj promenljivoj\n0. Kraj programa\n");
	while (flag) {
		printf("\nOdaberite stavku iz menija: ");
		scanf(" %c%*c", &u);
		switch (u) {
		case '1': { //unos izraza i formiranje stabla
			exp = InputExpression();
			printf("Izraz je unet.\n");
			tree = InitTree(exp);
			break;
		}
		case '2': { //ispis stabla
			printf("Napomena: pri ispisu stabla unarni operator ce biti ispisivan kao znak ~, dok ce log, exp, abs biti ispisivani kao l, e, a, respektivno\n\n");
			printf("Stablo:\n\n");
			PrintTree(tree);
			printf("\n");
			break;
		}
		case '3': { //ispis postfiksa
			printf("Napomena: pri ispisu izraza u postfiksnoj notaciji unarni operator ce biti ispisivan kao znak ~\n\n");
			printf("Postfiks: ");
			Postfix(tree);
			break;
		}
		case '4': { //zadavanje vrednosti
			for (int i = 0; i < 26; i++) variables[i] = 0;
			variables['E' - 'A'] = 2.71828;
			for (p = exp; *p != '\0'; p++) {
				if (*p >= 'A' && *p <= 'Z' && variables[*p - 'A'] == 0) {
					if (*p != 'E') {
						printf("%c? ", *p);
						scanf("%lf", &variables[*p - 'A']);
					}
				}
			}
			break;
		}
		case '5': { //racunanje izraza
			res = Calculate(tree, variables);
			printf("Rezultat je: %.2lf\n", res);
			break;
		}
		case '6': { //izvod
			printf("\nUnesite promenljivu po kojoj se radi izvod: ");
			scanf(" %c", &var);
			derivateTree = Derivate(tree, var);
			PrintTree(derivateTree);
			printf("\nAko zelite da pozovete neku od opcija iz menija za: \nizraz dobijen diferenciranjem ukucajte 0\n"
				"izraz pre poslednjeg diferenciranja ukucajte 1\n"
				"originalni izraz ukucajte 2: ");
			scanf(" %c", &u);
			switch (u) {
			case '0': 
				DeallocateTree(tree);
				tree = derivateTree; 
				break;
			case '1':
				break;
			case '2':
				tree = InitTree(exp);
				break;
			default:
				printf("Uneta opcija ne postoji, automatski odabrana opcija 1 tj vracanje na izraz pre poslednjeg diferenciranja");
			}
			break;
		}
		case '0': { //kraj programa
			flag = 0;
			free(exp);
			DeallocateTree(tree);
			if (tree != derivateTree)
				DeallocateTree(derivateTree);
			printf("Kraj programa\n");
			break;
		}
		default: {
			printf("Uneta opcija ne postoji u meniju, pokusajte ponovo\n");
		}
		}
	}
	return 0;
}
