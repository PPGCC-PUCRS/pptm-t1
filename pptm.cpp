#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <mpi.h>

const int MASTER_RANK = 0;
const int NUM_TASKS = 100;

void MasterProcess(int numProcesses) {
    auto start = std::chrono::high_resolution_clock::now();
    int taskList[NUM_TASKS];
    int nextTaskIndex = 0;
    int completedTasks = 0;
    int end_count = 0;
    MPI_Status status;

    // Inicialize a lista de tarefas
    for (int i = 0; i < NUM_TASKS; ++i) {
        taskList[i] = i;
    }

    while (true) {
        int slaveRank;      

        MPI_Recv(&slaveRank, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        
        if (nextTaskIndex < NUM_TASKS) {
            int task = taskList[nextTaskIndex++];
            MPI_Send(&task, 1, MPI_INT, slaveRank, 0, MPI_COMM_WORLD);
        } else {          
            // Não há mais tarefas, envie um sinal de término  
            end_count++;
            int terminationSignal = -1;      
            MPI_Send(&terminationSignal, 1, MPI_INT, slaveRank, 0, MPI_COMM_WORLD);
        }

        completedTasks++;

        if (end_count == (numProcesses-1)) {
            break;
        }

    }

    // Calcula o tempo total do processamento da tarefa
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double elapsed_seconds = duration.count();
    std::cout << "Duração da execução: " << std::fixed << std::setprecision(2) << elapsed_seconds << " segundos" << std::endl;
}

void SlaveProcess(int rank) {
    MPI_Status status;

    while (true) {
        int task;
        MPI_Send(&rank, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);
        MPI_Recv(&task, 1, MPI_INT, MASTER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (task == -1) {
            break;
        }

        // Simulação de processamento da tarefa
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Escravo " << rank << " processando a tarefa " << task << std::endl;
    }

    std::cout << "Escravo " << rank << " sem novas tarefas " << std::endl;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    int numProcesses;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    
    if (rank == MASTER_RANK) {
        MasterProcess(numProcesses);
    } else {
        SlaveProcess(rank);
    }
    
    MPI_Finalize();   

    return 0;
}