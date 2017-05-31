/****************************************************************************************************************/
/*                                                                                                              */ 
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.artelnics.com/opennn                                                                                   */
/*                                                                                                              */
/*   L O G I C A L   O P E R A T O R S   A P P L I C A T I O N                                                  */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */ 
/*   Artelnics - Making intelligent use of data                                                                 */
/*   robertolopez@artelnics.com                                                                                 */
/*                                                                                                              */  
/****************************************************************************************************************/

// This is a classical learning problem.

// System includes

#include <iostream>
#include <math.h>
#include <time.h>
#include <QDebug>

// OpenNN includes

#include "../../opennn/opennn.h"

using namespace OpenNN;

int main(void)
{
    try
    {

        Vector<unsigned> structure(5);
        structure[0] = 1;
        structure[1] = 2;
        structure[2] = 4;

        structure[3] = 2;
        structure[4] = 1;
        NeuralNetwork* myNN = new NeuralNetwork(2,4,2);

        Vector<double> in(2);
        in[0] = 0.5;
        in[1] = 0.25;

        Vector<double> out = myNN->calculate_outputs(in);

        std::cout<<out<<std::endl;
        std::cout<<"workin..."<<std::endl;











        int rank = 0;

#ifdef __OPENNN_MPI__

        int size = 1;

        MPI_Init(NULL,NULL);

        MPI_Comm_size(MPI_COMM_WORLD, &size);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#endif

        if(rank == 0)
        {
            std::cout << "OpenNN. Logical Operations Application." << std::endl;
        }

        srand((unsigned)time(NULL));

        // Global variables

        DataSet data_set;

        NeuralNetwork neural_network;

        LossIndex loss_index;

        TrainingStrategy training_strategy;

        // Local variables

        DataSet local_data_set;

        NeuralNetwork local_neural_network;

        LossIndex local_loss_index;

        TrainingStrategy local_training_strategy;

        if(rank == 0)
        {
            // Data set

            data_set.set_data_file_name("../data/logical_operations.dat");

            data_set.load_data();

            Variables* variables_pointer = data_set.get_variables_pointer();

            variables_pointer->set(2, 6);

            variables_pointer->set_name(0, "X");
            variables_pointer->set_name(1, "Y");
            variables_pointer->set_name(2, "AND");
            variables_pointer->set_name(3, "OR");
            variables_pointer->set_name(4, "NAND");
            variables_pointer->set_name(5, "NOR");
            variables_pointer->set_name(6, "XNOR");
            variables_pointer->set_name(7, "XNOR");

            const Matrix<std::string> inputs_information = variables_pointer->arrange_inputs_information();
            const Matrix<std::string> targets_information = variables_pointer->arrange_targets_information();

            // Neural network

            neural_network.set(2, 20, 6);

            Inputs* inputs_pointer = neural_network.get_inputs_pointer();

            inputs_pointer->set_information(inputs_information);

            Outputs* outputs_pointer = neural_network.get_outputs_pointer();

            outputs_pointer->set_information(targets_information);

            // Loss index

            loss_index.set_data_set_pointer(&data_set);
            loss_index.set_neural_network_pointer(&neural_network);

            // Training strategy

            training_strategy.set(&loss_index);
        }

#ifdef __OPENNN_MPI__
        MPI_Barrier(MPI_COMM_WORLD);

        local_data_set.set_MPI(&data_set);

        local_neural_network.set_MPI(&neural_network);

        local_loss_index.set_MPI(&local_data_set,&local_neural_network,&loss_index);

        local_training_strategy.set_MPI(&local_loss_index,&training_strategy);

        MPI_Barrier(MPI_COMM_WORLD);

        local_training_strategy.perform_training();
#else
        training_strategy.perform_training();
#endif

        if(rank == 0)
        {
#ifdef __OPENNN_MPI__
            neural_network.set_multilayer_perceptron_pointer(local_neural_network.get_multilayer_perceptron_pointer());
#endif
            // Save results

            data_set.save("../data/data_set.xml");

            neural_network.save("../data/neural_network.xml");

            loss_index.save("../data/loss_index.xml");

            training_strategy.save("../data/training_strategy.xml");

            // Print results to screen

            Vector<double> inputs(2, 0.0);
            Vector<double> outputs(6, 0.0);

            std::cout << "X Y AND OR NAND NOR XOR XNOR" << std::endl;

            inputs[0] = 1.0;
            inputs[1] = 1.0;

            outputs = neural_network.calculate_outputs(inputs);

            std::cout << inputs.calculate_binary() << " " << outputs.calculate_binary() << std::endl;

            inputs[0] = 1.0;
            inputs[1] = 0.0;

            outputs = neural_network.calculate_outputs(inputs);

            std::cout << inputs.calculate_binary() << " " << outputs.calculate_binary() << std::endl;

            inputs[0] = 0.0;
            inputs[1] = 1.0;

            outputs = neural_network.calculate_outputs(inputs);

            std::cout << inputs.calculate_binary() << " " << outputs.calculate_binary() << std::endl;

            inputs[0] = 0.0;
            inputs[1] = 0.0;

            outputs = neural_network.calculate_outputs(inputs);

            std::cout << inputs.calculate_binary() << " " << outputs.calculate_binary() << std::endl;
        }

#ifdef __OPENNN_MPI__

        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Finalize();

#endif

        return(0);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        return(1);
    }
}


// OpenNN: Open Neural Networks Library.
// Copyright (C) 2005-2015 Roberto Lopez
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
