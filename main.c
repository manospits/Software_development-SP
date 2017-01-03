#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CCindex.h"
#include "graph.h"
#include "error.h"
#include "jobscheduler.h"

#define OUTPUT_FILE_NAME "results.txt"
#define THREAD_POOL_SIZE 10

int main(int argc, char *argv[])
{
#ifdef VERBOSE_MODE
    if (argc != 3 && argc != 4)
    {
        fprintf(stderr, "Wrong number of arguments provided.\nExpected: ./<executable> <initial graph file> <workload file> <OPTIONAL: \"`wc -l <workload file>`\" >\nExiting...\n");
        return -1;
    }
#else
    if (argc != 3)
    {
        fprintf(stderr, "Wrong number of arguments provided.\nExpected: ./<executable> <initial graph file> <workload file>\nExiting...\n");
        return -1;
    }
#endif // VERBOSE_MODE
    int node1, node2, ret_val,type;
    unsigned long i;
#ifdef VERBOSE_MODE
    unsigned long lines = 0, current_percentage = 0;
    if (argc == 4) lines = strtoul(argv[3], NULL, 10);
#endif // VERBOSE_MODE
    FILE *initial_graph, *workload, *results;
    char command;
    pJobScheduler scheduler = NULL;
    pGraph graph = gCreateGraph();
    char typebuf[256];
    if (graph == NULL)
    {
        print_error();
        fprintf(stderr, "An error occurred during graph creation.\nExiting...\n");
        return -1;
    }/*
    if ((scheduler = initialize_scheduler(THREAD_POOL_SIZE, graph)) == NULL);
    {
        print_error();
        fprintf(stderr, "An error occurred during job scheduler initialization.\nExiting...\n");
        gDestroyGraph(&graph);
        exit(-1);
    }*/
    if ((initial_graph = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "Error opening initial graph file '%s'\nExiting...\n", argv[1]);
        destroy_scheduler(scheduler);
        gDestroyGraph(&graph);
        return -1;
    }
    if ((workload = fopen(argv[2], "r")) == NULL)
    {
        fprintf(stderr, "Error opening workload file '%s'\nExiting...\n", argv[2]);
        fclose(initial_graph);
        destroy_scheduler(scheduler);
        gDestroyGraph(&graph);
        return -1;
    }
    if ((results = fopen(OUTPUT_FILE_NAME, "w")) == NULL)
    {
        fprintf(stderr, "Error opening (or creating) results file '%s'\nExiting...\n", OUTPUT_FILE_NAME);
        fclose(initial_graph);
        fclose(workload);
        destroy_scheduler(scheduler);
        gDestroyGraph(&graph);
        return -1;
    }
    // read initial graph
    puts("Reading initial graph...");
    for (i = 1 ;; ++i)
    {
        command = fgetc(initial_graph);
        //printf("check: '%c'\n", command); // DEBUG
        if (/*(command = fgetc(initial_graph))*/command == 'S')
            break;
        ungetc(command, initial_graph);
        if (fscanf(initial_graph, "%d %d\n", &node1, &node2) != 2)
        {
            fprintf(stderr, "Error reading initial graph file: did not read two integers from line %lu\nExiting...\n", i);
            fclose(initial_graph);
            fclose(workload);
            fclose(results);
            destroy_scheduler(scheduler);
            gDestroyGraph(&graph);
            return -1;
        }
        //printf("%d %d\n", node1, node2); // DEBUG
        ret_val = gAddEdge(graph, node1, node2);
        if (ret_val < 0)
        {
            print_error();
            fprintf(stderr, "Error(s) found while processing initial graph file (line %lu)\nExiting...\n", i);
            fclose(initial_graph);
            fclose(workload);
            fclose(results);
            destroy_scheduler(scheduler);
            gDestroyGraph(&graph);
            return -1;
        }
    }
    puts("Reading complete.");
    puts("Building assistant structures/indexes...");
    if(fscanf(workload, "%s", typebuf) == EOF)
    {
        fprintf(stderr, "Error reading type in workload file\n");
        fclose(initial_graph);
        fclose(workload);
        fclose(results);
        destroy_scheduler(scheduler);
        gDestroyGraph(&graph);
        return -1;
    }
    if(strcmp(typebuf, "DYNAMIC") == 0)
    {
        if (create_indexes(graph,DYNAMIC) < 0)
        {
            print_error();
            fprintf(stderr, "Error creating assistant structures/indexes for dynamic graph\n");
            fclose(initial_graph);
            fclose(workload);
            fclose(results);
            destroy_scheduler(scheduler);
            gDestroyGraph(&graph);
            return -1;
        }
        type=DYNAMIC;
    }
    else if(strcmp(typebuf, "STATIC") == 0)
    {
        if (create_indexes(graph,STATIC) < 0)
        {
            print_error();
            fprintf(stderr, "Error creating assistant structures/indexes for static graph\n");
            fclose(initial_graph);
            fclose(workload);
            fclose(results);
            destroy_scheduler(scheduler);
            gDestroyGraph(&graph);
            return -1;
        }
        type=STATIC;
    }
    else
    {
        fprintf(stderr, "Error: unrecognized graph type (neither of DYNAMIC, STATIC). Exiting...\n");
        fclose(initial_graph);
        fclose(workload);
        fclose(results);
        destroy_scheduler(scheduler);
        gDestroyGraph(&graph);
        return -1;
    }
    puts("Building complete.");
    puts("Processing workload...");
    if(fgets(typebuf, 255, workload) == NULL)
    {
        fprintf(stderr, "Error reading in workload file\n");
        fclose(initial_graph);
        fclose(workload);
        fclose(results);
        destroy_scheduler(scheduler);
        gDestroyGraph(&graph);
        return -1;
    }
#ifdef VERBOSE_MODE
    if (lines) {printf("0%%");fflush(stdout);}
#endif // VERBOSE_MODE
    if(type==DYNAMIC){
        for (i = 1 ; !feof(workload) ; ++i)
        {
            command = fgetc(workload);
            //printf("check: '%c'\n", command); // DEBUG
            if (command == EOF)
                break;
            if (command == 'F')
            {
                rebuild(graph);
                if (fgets(typebuf, 255, workload) == NULL)
                    break;
                else    // take the '\n' and continue
                    continue;
            }
            ungetc(command, workload);
            if (fscanf(workload, "%c %d %d\n", &command, &node1, &node2) != 3)
            {
                fprintf(stderr, "Error reading workload file: did not read a character and two integers from line %lu\nExiting...\n", i);
                fclose(initial_graph);
                fclose(workload);
                fclose(results);
                destroy_scheduler(scheduler);
                gDestroyGraph(&graph);
                return -1;
            }
            /*printf("%c %d %d\n", command, node1, node2); // DEBUG*/
            if (command == 'A')
            {
                ret_val = gAddEdge(graph, node1, node2);
                if (ret_val < 0)
                {
                    print_error();
                    fprintf(stderr, "Error(s) found while processing workload file (line %lu)\nExiting...\n", i);
                    fclose(initial_graph);
                    fclose(workload);
                    fclose(results);
                    destroy_scheduler(scheduler);
                    gDestroyGraph(&graph);
                    return -1;
                }
            }
            else if (command == 'Q')
            {
                ret_val = gFindShortestPath(graph, node1, node2, BIDIRECTIONAL_BFS);
                if (ret_val >= 0)
                {
                    fprintf(results, "%d\n", ret_val);
                }
                else if (ret_val == GRAPH_SEARCH_PATH_NOT_FOUND)
                {
                    fprintf(results, "-1\n");
                }
                else // ret_val < 0, an error occurred (not GRAPH_SEARCH_PATH_NOT_FOUND)
                {
                    print_error();
                    fprintf(stderr, "Error(s) found while processing workload file (line %lu)\nExiting...\n", i);
                    fclose(initial_graph);
                    fclose(workload);
                    fclose(results);
                    destroy_scheduler(scheduler);
                    gDestroyGraph(&graph);
                    return -1;
                }
            }
            else
            {
                fprintf(stderr, "Error reading workload file: command '%c' not recognized (line %lu)\nExiting...\n", command, i);
                fclose(initial_graph);
                fclose(workload);
                fclose(results);
                destroy_scheduler(scheduler);
                gDestroyGraph(&graph);
                return -1;
            }
#ifdef VERBOSE_MODE
            if (lines)
            {
                if ((i*100)/lines > current_percentage)
                {
                    current_percentage = (i*100)/lines;
                    printf("\b\b");
                    if (current_percentage > 9) printf("\b");
                    printf("%lu%%", current_percentage);
                    fflush(stdout);
                }
            }
#endif // VERBOSE_MODE
        }
    }
    else if(type==STATIC){
        for (i = 1 ; !feof(workload) ; ++i)
        {
            command = fgetc(workload);
            //printf("check: '%c'\n", command); // DEBUG
            if (command == EOF)
                break;
            if (command == 'F')
            {
                if (fgets(typebuf, 255, workload) == NULL)
                    break;
                else    // take the '\n' and continue
                    continue;
            }
            ungetc(command, workload);
            if (fscanf(workload, "%c %d %d\n", &command, &node1, &node2) != 3)
            {
                fprintf(stderr, "Error reading workload file: did not read a character and two integers from line %lu\nExiting...\n", i);
                fclose(initial_graph);
                fclose(workload);
                fclose(results);
                destroy_scheduler(scheduler);
                gDestroyGraph(&graph);
                return -1;
            }
            /*printf("%c %d %d\n", command, node1, node2); // DEBUG*/
            if (command == 'Q')
            {
                ret_val = gFindShortestPath(graph, node1, node2, BIDIRECTIONAL_BFS);
                if (ret_val >= 0)
                {
                    fprintf(results, "%d\n", ret_val);
                }
                else if (ret_val == GRAPH_SEARCH_PATH_NOT_FOUND)
                {
                    fprintf(results, "-1\n");
                }
                else // ret_val < 0, an error occurred (not GRAPH_SEARCH_PATH_NOT_FOUND)
                {
                    print_error();
                    fprintf(stderr, "Error(s) found while processing workload file (line %lu)\nExiting...\n", i);
                    fclose(initial_graph);
                    fclose(workload);
                    fclose(results);
                    destroy_scheduler(scheduler);
                    gDestroyGraph(&graph);
                    return -1;
                }
            }
            else
            {
                fprintf(stderr, "Error reading workload file: command '%c' not recognized (line %lu)\nExiting...\n", command, i);
                fclose(initial_graph);
                fclose(workload);
                fclose(results);
                destroy_scheduler(scheduler);
                gDestroyGraph(&graph);
                return -1;
            }
#ifdef VERBOSE_MODE
            if (lines)
            {
                if ((i*100)/lines > current_percentage)
                {
                    current_percentage = (i*100)/lines;
                    printf("\b\b");
                    if (current_percentage > 9) printf("\b");
                    printf("%lu%%", (i*100)/lines);
                    fflush(stdout);
                }
            }
#endif // VERBOSE_MODE
        }

    }
#ifdef VERBOSE_MODE
    if ((i-1)/lines != 1)printf("\b\b\b100%%\n");
#endif // VERBOSE_MODE
    puts("Processing complete.");
    printf("Results can be found in file '%s'.\n", OUTPUT_FILE_NAME);
    destroy_scheduler(scheduler);
    gDestroyGraph(&graph);
    fclose(initial_graph);
    fclose(workload);
    fclose(results);
    return 0;
}
