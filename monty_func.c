#include "monty.h"
/**
 * read_file - reads bytecode file
 * @filename: file pathname
 * @stack: pointer to stack-top
 */
void read_file(char *filename, stack_t **stack)
{
	char *line;
	size_t k = 0;
	int lineno = 1;
	instruct_func s;
	int view;
	int rd;

	var_global.file = fopen(filename, "r");

	if (var_global.file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	while ((rd = getline(&var_global.buffer, &k, var_global.file)) != -1)
	{
		line = parse_line(var_global.buffer, stack, lineno);
		if (line == NULL || line[0] == '#')
		{
			lineno++;
			continue;
		}
		s = get_op_func(line);
		if (s == NULL)
		{
			fprintf(stderr, "L%d: unknown instruction %s\n", lineno, line);
			exit(EXIT_FAILURE);
		}
		s(stack, lineno);
		lineno++;
	}
	free(var_global.buffer);
	view = fclose(var_global.file);
	if (view == -1)
		exit(-1);
}

/**
 * get_op_func - checks code and returns function
 * @str: code
 * Return: functions else NULL
 */
instruct_func get_op_func(char *str)
{
	int k;

	instruction_t instruct[] = {
		{"push", _push},
		{"pall", _pall},
		{"pint", _pint},
		{"pop", _pop},
		{"swap", _swap},
		{"add", _add},
		{"nop", _nop},
		{"sub", _sub},
		{"mul", _mul},
		{"div", _div},
		{"mod", _mod},
		{"pchar", _pchar},
		{"pstr", _pstr},
		{"rotl", _rotl},
		{"rotr", _rotr},
		{NULL, NULL},
	};

	k = 0;
	while (instruct[k].f != NULL && strcmp(instruct[k].opcode, str) != 0)
	{
		k++;
	}
	return (instruct[k].f);
}

/**
 * isnumber - checks if string is a number
 * @str: string being checked
 * Return: 1 if string is number else 0
 */
int isnumber(char *str)
{
	unsigned int k;

	if (str == NULL)
		return (0);
	k = 0;
	while (str[k])
	{
		if (str[0] == '-')
		{
			k++;
			continue;
		}
		if (!isdigit(str[k]))
			return (0);
		k++;
	}
	return (1);
}

/**
 * parse_line - parses a line for an opcode and arguments
 * @line: line to parse
 * @stack: stack pointer to head
 * @line_number: current line number
 * Return: operation code else NULL
 */
char *parse_line(char *line, stack_t **stack, unsigned int line_number)
{
	char *op_code, *push, *arg;
	(void)stack;

	push = "push";
	op_code = strtok(line, "\n ");
	if (op_code == NULL)
		return (NULL);
	if (strcmp(op_code, push) == 0)
	{
		arg = strtok(NULL, "\n ");
		if (isnumber(arg) == 1 && arg != NULL)
		{
			var_global.push_arg = atoi(arg);
		}
		else
		{
			fprintf(stderr, "L%d: usage: push integer\n", line_number);
			exit(EXIT_FAILURE);
		}
	}
	return (op_code);
}
