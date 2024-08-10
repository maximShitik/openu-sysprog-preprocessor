#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#include "macros.h"

/**
 * @brief a struct that holds the opcode number i nthe AST and the name of the opcode
 *
 */
typedef struct
{
    const char *name;
    int opcode;
} opcode_map;

/**
 * @brief an enum for each command group
 * one goup - with one operand
 * two group - with two operands
 * no operands - for commands with no operands for example "stop"
 *
 */
enum command_groug
{
    no_operands,
    one_group,
    two_group
};

/**
 * @brief A struct for counting the number of words in the line and hold each worn in diffrent index in the array
 *
 */
struct sep_line
{
    char *line[MAX_ADDRESS];
    int line_number;
};

/**
 * @brief an enum for the return types of the prosses_line function
 *
 */
typedef enum return_types
{
    error = -1,
    instruction = 2,
    command,
    registerr,
    Entry,
    Extrn

} return_types;

/**
 * @brief The main data structure for lexical analysis.
 * 
 * This ast(Abstract Syntax Tree) structure is designed to hold all the data 
 * from a line after lexical analysis. It captures different types of lines 
 * (instruction, command, etc.) and stores the relevant data in a union 
 * structure.
 */
typedef struct ast
{
    /**
     * @brief Enumeration of possible line types.
     * 
     * The type of line that has been analyzed:
     * error_line: Indicates that there was an error in the line.
     * inst_line: Represents an instruction line.
     * command_line: Represents a command line.
     * empty_line: Represents an empty line.
     */
    enum line_type
    {
        error_line = 1,  
        inst_line,       
        command_line,    
        empty_line       
    } line_type;         

    /**
     * @brief Union holding data specific to the line type.
     * 
     * Depending on the type of line, different data structures are used 
     * to store the relevant information:
     * inst: Used for instruction lines (e.g., data, string, entry, extern).
     * command: Used for command lines (e.g., mov,cmp,add,etc.).
     */
    union line_type_data
    {
        /**
         * @brief Data structure for instruction lines.
         * 
         * This structure captures details of an instruction line, including:
         * inst_type: The type of instruction (e.g., label, string, entry, extern, data).
         * label_array: Array for storing labels associated with entry or extern instructions.
         * data_array: Array for storing data values.
         * string_array: Array for storing strings.
         * data_counter: Counter for the number of data elements or strings.
         */
        struct inst
        {
            /**
             * @brief Enumeration of instruction types.
             * 
             * The type of instruction being processed:
             */
            enum inst_type
            {
                label = 1,  
                string,     
                entry,      
                extrn,      
                data       
            } inst_type;   

            char *label_array[LABEL_ARRAY_SIZE];
            int data_array[MAX_LINE];            
            char *string_array[STRING_ARRAY_SIZE]; 
            int data_counter;                    
        } inst;

        /**
         * @brief Data structure for command lines.
         * 
         * This structure captures details of a command line, including:
         * opcode: The operation code of the command.
         * opcode_type: The types of operands used by the command.
         */
        struct command
        {
        
            enum
            {
                mov = 0,  
                cmp,      
                add,      
                sub,      
                lea,      
                clr,      
                nt,      
                inc,      
                dec,     
                jmp,
                bne,      
                red,      
                prn,      
                jsr,      
                rts,     
                stop     
            } opcode;     

            /**
             * @brief Structure for the command operands.
             * 
             * Contains the type and value of the operands used in the command:
             * command_type: Type of the operand (none, number, label, register, etc.).
             * labell: Array for storing label operands.
             * numberr: The numeric value of the operand.
             * regg: The register used as the operand.
             */
            struct
            {
                /**
                 * @brief Enumeration of operand types.
                 * 
                 * The type of operand:
                 * none: No operand.
                 * number: A numeric operand.
                 * label: A label operand.
                 * reg: A register operand.
                 * reg_direct: Direct register addressing.
                 */
                enum
                {
                    none,       
                    number,     
                    lable,      
                    reg,        
                    reg_direct  
                } command_type; 

                char *labell[2]; 
                int numberr;     
                int regg;        
            } opcode_type[2];  
        } command;

    } line_type_data;          

    /**
     * @brief Structure for storing error information.
     * 
     * This structure captures details of any errors found during lexical analysis.
     */
    struct
    {
        char type[MAX_DATA]; 
    } error;

    /**
     * @brief Structure for the ARE type.
     * 
     * Captures the ARE type information for the current line.
     */
    struct
    {
    
        enum
        {
            E,  /*External ARE type. */
            R,  /*entry ARE type. */
            A   /*regular ARE type. */
        } are_type; 
    } are;

    char label_name[MAX_LABEL]; /* The label name associated with the line. */
    int argument_count;         /* The number of arguments in the line. */

} ast;


/**
 * @brief a struct that holds the data for the pre-prosses
 * each node in the line_node struct holds a line data and a pointer to the next node
 * if the macro has serval lines it will be stored in this struct
 *
 */
typedef struct line_node
{
    char *line_data;
    struct line_node *next;
} line_node;

/**
 * @brief hash table struct for the pre-prosses
 * each node in the hash table holds a line_node struct with the line data if the line have a macro defenition and a pointer to the next node
 *
 */
typedef struct hash
{
    char *name;
    line_node *data;
    struct hash *next;

} hash;

/**
 * @brief Represents a symbol in the assembler.
 *
 * The symbol struct is used to store information about symbols encountered during
 * the assembly process. Each symbol has a name, an address, a type, and a pointer
 * to the next symbol in the list.
 * The enum categorizes the symbol based on its role in the program:
 * code_type: The symbol is associated with a code segment.
 * data_type: The symbol is associated with a data segment.
 * entry_type: The symbol is marked as an entry point.
 * extrn_type: The symbol is an external reference.
 * entry_code_type: The symbol is an entry point and if its initilized in the file it will be changed to this.
 * entry_data_type: The symbol is an entry point and if its initilized in the file it will be changed to this.
 */
struct symbol
{
    char symbol_name[MAX_LINE]; /*The name of the symbol.*/
    int address;                /*The address of the symbol in memory.*/

    enum symbol_type
    {
        code_type,       
        data_type,       
        entry_type,     
        extrn_type,      
        entry_code_type, 
        entry_data_type  
    } symbol_type;       

    struct symbol *next; /*Pointer to the next symbol in the list.*/
};

/**
 * @brief a struct that holds the address of the external symbols in a linked list
 * this struct used when the same extern symbol has another address in the file
 * 
 */
struct address
{
    int address;
    struct address *next_address;
    struct address *prev_address;
};

/**
 * @brief a linked list for the external symbols
 * each node in the list holds the name of the external symbol and a pointer to the next and the prev node
 * the address counter counts how many addreses has the same extern symbol
 * the address head holds the first address of the extern symbol
 */
struct ext
{
    char ext_name[MAX_LINE];
    struct address *address_head;
    int address_counter;
    struct ext *next;
    struct ext *prev;
};

/**
 * @brief a lined list to hold the entry symbols
 * 
 */
struct entry
{
    char entry_name[MAX_LINE];
    int address;
    struct entry *next;
    struct entry *prev;
};

/**
 * @brief Represents a translation unit in an assembler.
 * 
 * The translation_unit structure is designed to store various components 
 * of an assembler’s translation process, including code, data, symbols, 
 * external references, and entry points.
 */
struct translation_unit
{
    int code_array[MAX_ADDRESS];    /*Array for storing the machine code.*/
    int data_array[MAX_ADDRESS];    /*Array for storing the data section.*/
    
    struct symbol *symbol_table;    /*Pointer to the symbol table, which holds symbols and their attributes.*/
    
    struct ext *ext_table;          /*Pointer to the table of external references.*/
    
    int IC;                         /*tracking the number of instructions. */
    int DC;                         /*tracking the amount of data.*/
    
    struct entry *entry_table;      /*Pointer to the table of entry points.*/
    
    int entry_count;                /*Number of entries in the entry table. */
    int exter_count;                /*Number of external references in the external table. */
};


/**
 * @brief a struct that holds the original line number and the expanded line number
 * used for tracking the offset of the lines in the am file after spreading the macros .
 * if ther eis an error in the file we can track the error to the original line number
 */
typedef struct
{
    int original_line_number;
    int expanded_line_number;

} line_mapping;


#endif