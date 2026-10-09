#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int main()
{
    int n, m;
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int max[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int safeSequence[MAX_PROCESSES];
    int i, j;
    int count = 0;

    printf("========================================\n");
    printf("       BANKER'S ALGORITHM CHECKER       \n");
    printf("========================================\n\n");

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    printf("\n--- Enter Available Resources ---\n");
    for (i = 0; i < m; i++)
    {
        printf("Resource R%d: ", i);
        scanf("%d", &available[i]);
    }

    printf("\n--- Enter Maximum Resource Matrix ---\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i);
        for (j = 0; j < m; j++) scanf("%d", &max[i][j]);
    }

    printf("\n--- Enter Allocation Matrix ---\n");
    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i);
        for (j = 0; j < m; j++) scanf("%d", &allocation[i][j]);
    }

    // Calculate Need = Max - Allocation
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // --- VISUALIZATION: SYSTEM STATE TABLE ---
    printf("\n=========================================================\n");
    printf("                      SYSTEM STATE                       \n");
    printf("=========================================================\n");
    
    printf("Available Resources: ");
    for (i = 0; i < m; i++) printf("%d ", available[i]);
    printf("\n\n");

    // Dynamic Header based on resource count
    printf("%-8s | %-12s | %-12s | %-12s\n", "Process", "Allocation", "Maximum", "Need");
    printf("---------------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        printf("P%-7d | ", i);
        
        for (j = 0; j < m; j++) printf("%2d ", allocation[i][j]);
        for (j = m; j < 4; j++) printf("   "); // Padding for alignment
        printf(" | ");

        for (j = 0; j < m; j++) printf("%2d ", max[i][j]);
        for (j = m; j < 4; j++) printf("   ");
        printf(" | ");

        for (j = 0; j < m; j++) printf("%2d ", need[i][j]);
        printf("\n");
    }
    printf("=========================================================\n\n");

    // Copy Available into Work
    for (i = 0; i < m; i++) work[i] = available[i];

    printf("--- EXECUTION TRACE ---\n");

    // Banker's Safety Algorithm
    while (count < n)
    {
        bool found = false;
        for (i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                bool canExecute = true;
                for (j = 0; j < m; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        canExecute = false;
                        break;
                    }
                }

                if (canExecute)
                {
                    // Execute and release resources
                    for (j = 0; j < m; j++) work[j] += allocation[i][j];
                    
                    finish[i] = true;
                    safeSequence[count++] = i;
                    found = true;

                    // Trace Visualization
                    printf("[+] P%d executes. New Work Array: [ ", i);
                    for (j = 0; j < m; j++) printf("%d ", work[j]);
                    printf("]\n");
                }
            }
        }
        if (!found) break; // System is stuck
    }

    // --- FINAL VERDICT ---
    printf("\n========================================\n");
    if (count == n)
    {
        printf("       SYSTEM IS IN A SAFE STATE        \n");
        printf("========================================\n");
        printf("Safe Sequence: ");
        for (i = 0; i < n; i++)
        {
            printf("P%d", safeSequence[i]);
            if (i != n - 1) printf(" -> ");
        }
        printf("\n");
    }
    else
    {
        printf("      SYSTEM IS IN AN UNSAFE STATE      \n");
        printf("========================================\n");
        printf("Deadlock detected. No safe sequence exists.\n");
    }

    return 0;
}
