/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.artelnics.com/opennn                                                                                   */
/*                                                                                                              */
/*   A I R F O I L   S E L F - N O I S E   A P P L I C A T I O N                                                */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   Artelnics - The artificial intelligence company                                                            */
/*   robertolopez@artelnics.com                                                                                 */
/*                                                                                                              */
/****************************************************************************************************************/

// This is a function regression problem.

// System includes

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <time.h>

// OpenNN includes

#include "../../opennn/opennn.h"

using namespace OpenNN;

int main(void)
{
    try
    {
        int rank = 0;

#ifdef __OPENNN_MPI__

        int size = 1;

        MPI_Init(NULL,NULL);

        MPI_Comm_size(MPI_COMM_WORLD, &size);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#endif

        if(rank == 0)
        {
            std::cout << "OpenNN. Airfoil Self-Noise Application." << std::endl;
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

            data_set.set_data_file_name("../data/airfoil_self_noise.dat");

            data_set.set_separator("Tab");

            data_set.load_data();

            // Variables

            Variables* variables_pointer = data_set.get_variables_pointer();

            Vector< Variables::Item > variables_items(6);

            variables_items[0].name = "frequency";
            variables_items[0].units = "hertzs";
            variables_items[0].use = Variables::Input;

            variables_items[1].name = "angle_of_attack";
            variables_items[1].units = "degrees";
            variables_items[1].use = Variables::Input;

            variables_items[2].name = "chord_length";
            variables_items[2].units = "meters";
            variables_items[2].use = Variables::Input;

            variables_items[3].name = "free_stream_velocity";
            variables_items[3].units = "meters per second";
            variables_items[3].use = Variables::Input;

            variables_items[4].name = "suction_side_displacement_thickness";
            variables_items[4].units = "meters";
            variables_items[4].use = Variables::Input;

            variables_items[5].name = "scaled_sound_pressure_level";
            variables_items[5].units = "decibels";
            variables_items[5].use = Variables::Target;

            variables_pointer->set_items(variables_items);

            const Matrix<std::string> inputs_information = variables_pointer->arrange_inputs_information();
            const Matrix<std::string> targets_information = variables_pointer->arrange_targets_information();

            // Instances

            Instances* instances_pointer = data_set.get_instances_pointer();

            instances_pointer->split_random_indices();

            const Vector< Statistics<double> > inputs_statistics = data_set.scale_inputs_minimum_maximum();
            const Vector< Statistics<double> > targets_statistics = data_set.scale_targets_minimum_maximum();

            // Neural network

            const size_t inputs_number = variables_pointer->count_inputs_number();
            const size_t hidden_perceptrons_number = 9;
            const size_t outputs_number = variables_pointer->count_targets_number();

            neural_network.set(inputs_number, hidden_perceptrons_number, outputs_number);

            Inputs* inputs = neural_network.get_inputs_pointer();

            inputs->set_information(inputs_information);

            Outputs* outputs = neural_network.get_outputs_pointer();

            outputs->set_information(targets_information);

            neural_network.construct_scaling_layer();

            ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();

            scaling_layer_pointer->set_statistics(inputs_statistics);

            scaling_layer_pointer->set_scaling_method(ScalingLayer::NoScaling);

            neural_network.construct_unscaling_layer();

            UnscalingLayer* unscaling_layer_pointer = neural_network.get_unscaling_layer_pointer();

            unscaling_layer_pointer->set_statistics(targets_statistics);

            unscaling_layer_pointer->set_unscaling_method(UnscalingLayer::NoUnscaling);

            // Loss index

            loss_index.set_data_set_pointer(&data_set);
            loss_index.set_neural_network_pointer(&neural_network);

            loss_index.set_regularization_type(LossIndex::NEURAL_PARAMETERS_NORM);

            // Training strategy object

            training_strategy.set(&loss_index);

            QuasiNewtonMethod* quasi_Newton_method_pointer = training_strategy.get_quasi_Newton_method_pointer();

            quasi_Newton_method_pointer->set_maximum_iterations_number(1000);
            quasi_Newton_method_pointer->set_display_period(10);

            quasi_Newton_method_pointer->set_minimum_loss_increase(1.0e-6);

            quasi_Newton_method_pointer->set_reserve_loss_history(true);
        }

#ifdef __OPENNN_MPI__
        MPI_Barrier(MPI_COMM_WORLD);

        local_data_set.set_MPI(&data_set);

        local_neural_network.set_MPI(&neural_network);

        local_loss_index.set_MPI(&local_data_set,&local_neural_network,&loss_index);

        local_training_strategy.set_MPI(&local_loss_index,&training_strategy);

        MPI_Barrier(MPI_COMM_WORLD);

        TrainingStrategy::Results training_strategy_results = local_training_strategy.perform_training();
#else
        TrainingStrategy::Results training_strategy_results = training_strategy.perform_training();
#endif

        if(rank == 0)
        {
#ifdef __OPENNN_MPI__
            neural_network.set_multilayer_perceptron_pointer(local_neural_network.get_multilayer_perceptron_pointer());
#endif
            // Testing analysis

            TestingAnalysis testing_analysis(&neural_network, &data_set);

            TestingAnalysis::LinearRegressionResults linear_regression_results = testing_analysis.perform_linear_regression_analysis();

            // Save results

            ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();
            UnscalingLayer* unscaling_layer_pointer = neural_network.get_unscaling_layer_pointer();

            scaling_layer_pointer->set_scaling_method(ScalingLayer::MinimumMaximum);
            unscaling_layer_pointer->set_unscaling_method(UnscalingLayer::MinimumMaximum);

            data_set.save("../data/data_set.xml");

            neural_network.save("../data/neural_network.xml");
            neural_network.save_expression("../data/expression.txt");

            loss_index.save("../data/loss_index.xml");

            training_strategy.save("../data/training_strategy.xml");
            training_strategy_results.save("../data/training_strategy_results.dat");

            linear_regression_results.save("../data/linear_regression_analysis_results.dat");
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
// Copyright (C) 2005-2015 Roberto Lopez.
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
