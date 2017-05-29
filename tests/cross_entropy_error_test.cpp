/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: Open Neural Networks Library                                                                       */
/*   www.opennn.net                                                                                             */
/*                                                                                                              */
/*   C R O S S   E N T R O P Y   E R R O R   T E S T   C L A S S                                                */
/*                                                                                                              */
/*   Roberto Lopez                                                                                              */
/*   Artelnics - Making intelligent use of data                                                                 */
/*   robertolopez@artelnics.com                                                                                 */
/*                                                                                                              */
/****************************************************************************************************************/

// OpenNN includes

#include "cross_entropy_error_test.h"

using namespace OpenNN;

// GENERAL CONSTRUCTOR

CrossEntropyErrorTest::CrossEntropyErrorTest(void) : UnitTesting() 
{
}


// DESTRUCTOR

CrossEntropyErrorTest::~CrossEntropyErrorTest(void)
{
}


// METHODS


// @todo

void CrossEntropyErrorTest::test_calculate_loss(void)
{
   message += "test_calculate_loss\n";

   NeuralNetwork nn;
   Vector<double> parameters;

   MultilayerPerceptron* mlpp;

   DataSet ds;
   
   CrossEntropyError cee(&nn, &ds);

   double cross_entropy_error;

   // Test

   nn.set(1, 1);

   mlpp = nn.get_multilayer_perceptron_pointer();

   mlpp->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);

   nn.initialize_parameters(0.0);
   parameters = nn.arrange_parameters();
   
   ds.set(1,1,1);
   ds.initialize_data(0.0);

   cross_entropy_error = cee.calculate_error();

   assert_true(cross_entropy_error == cee.calculate_error(parameters), LOG);

   // Test

   nn.set(1, 1);

   mlpp = nn.get_multilayer_perceptron_pointer();

   mlpp->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);

   nn.randomize_parameters_normal();

   parameters = nn.arrange_parameters();

   ds.set(1, 1, 1);
   ds.randomize_data_normal();

   assert_true(cee.calculate_error() == cee.calculate_error(parameters), LOG);

   // Test

   nn.set(1, 1);

   mlpp = nn.get_multilayer_perceptron_pointer();

   mlpp->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);

   nn.randomize_parameters_normal();

   parameters = nn.arrange_parameters();

   ds.set(1, 1, 1);
   ds.randomize_data_normal();

   assert_true(cee.calculate_error() != cee.calculate_error(parameters*2.0), LOG);

   // Test

   nn.set(3, 1);

   mlpp = nn.get_multilayer_perceptron_pointer();

   mlpp->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);

   nn.randomize_parameters_normal();

   ds.set(50,3,1);
   ds.randomize_data_normal();

   assert_true(cee.calculate_error() > 0, LOG);

   // Test

   nn.set(3,3,3);

   mlpp = nn.get_multilayer_perceptron_pointer();

   mlpp->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);
   mlpp->get_layer_pointer(1)->set_activation_function(Perceptron::Logistic);

   nn.initialize_parameters(0);

   ds.set(50,3,3);
   ds.randomize_data_normal();

   assert_true(cee.calculate_error() > 0, LOG);

}


void CrossEntropyErrorTest::test_calculate_selection_loss(void)   
{
   message += "test_calculate_selection_loss\n";

   NeuralNetwork nn;
   DataSet ds;
   
   CrossEntropyError cee(&nn, &ds);

   double selection_objective;

   // Test

   nn.set(1, 1);
   nn.get_multilayer_perceptron_pointer()->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);
   nn.initialize_parameters(0.0);
   
   ds.set(1,1,1);
   ds.get_instances_pointer()->set_selection();
   ds.initialize_data(0.0);
   
   selection_objective = cee.calculate_selection_error();

   assert_true(selection_objective > 0.0, LOG);
}


void CrossEntropyErrorTest::test_calculate_minimum_loss(void)
{
    message += "test_calculate_minimum_loss\n";

    NeuralNetwork nn;
    DataSet ds;

    CrossEntropyError cee(&nn, &ds);

    double minimum_loss;
    double loss;

    // Test

    nn.set(1, 1);
    nn.get_multilayer_perceptron_pointer()->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);
    nn.randomize_parameters_normal();

    ds.set(20,1,1);
    ds.randomize_data_uniform(0,1);

    loss = cee.calculate_error();
    minimum_loss = cee.calculate_minimum_loss();

    assert_true(minimum_loss <= loss, LOG);
    assert_true(minimum_loss > 0, LOG);
}


void CrossEntropyErrorTest::test_calculate_minimum_selection_loss(void)
{
    message += "test_calculate_minimum_selection_loss\n";

    NeuralNetwork nn;
    DataSet ds;

    CrossEntropyError cee(&nn, &ds);

    double minimum_selection_loss;
    double selection_loss;

    // Test

    nn.set(1, 1);
    nn.get_multilayer_perceptron_pointer()->get_layer_pointer(0)->set_activation_function(Perceptron::Logistic);

    ds.set(20,1,1);
    ds.get_instances_pointer()->set_selection();
    ds.randomize_data_uniform(0,1);

    selection_loss = cee.calculate_selection_error();
    minimum_selection_loss = cee.calculate_minimum_selection_error();

    assert_true(minimum_selection_loss <= selection_loss, LOG);
    assert_true(minimum_selection_loss > 0, LOG);
}


void CrossEntropyErrorTest::test_calculate_gradient(void)
{
   message += "test_calculate_gradient\n";

   NumericalDifferentiation nd;

   NeuralNetwork nn;
   Vector<size_t> architecture;

   Vector<double> parameters;

   DataSet ds;

   CrossEntropyError cee(&nn, &ds);

   Vector<double> gradient;
   Vector<double> numerical_gradient;

   // Test

   nn.set(1, 1);

   nn.initialize_parameters(0.0);

   ds.set(1,1,1);

   ds.initialize_data(0.0);

   gradient = cee.calculate_gradient();

   assert_true(gradient.size() == nn.count_parameters_number(), LOG);

   // Test 

   nn.set(3, 4, 2);

   nn.initialize_parameters(0.0);

   ds.set(5,3,2);
   cee.set(&nn, &ds);
   ds.initialize_data(0.0);

   gradient = cee.calculate_gradient();

   assert_true(gradient.size() == nn.count_parameters_number(), LOG);

   // Test

   architecture.set(3);
   architecture[0] = 2;
   architecture[1] = 1;
   architecture[2] = 3;

   nn.set(architecture);

   nn.initialize_parameters(0.0);

   ds.set(5,2,3);
   cee.set(&nn, &ds);
   ds.initialize_data(0.0);

   gradient = cee.calculate_gradient();

   assert_true(gradient.size() == nn.count_parameters_number(), LOG);

   // Test

   nn.set(1, 1, 1);

   nn.initialize_parameters(0.0);

   ds.set(1,1,1);

   ds.initialize_data(0.0);

   gradient = cee.calculate_gradient();

   assert_true(gradient.size() == nn.count_parameters_number(), LOG);


   // Test 

   nn.set(3,4,2);

   nn.initialize_parameters(0.0);

   ds.set(5,3,2);
   cee.set(&nn, &ds);
   ds.initialize_data(0.0);

   gradient = cee.calculate_gradient();

   assert_true(gradient.size() == nn.count_parameters_number(), LOG);


   // Test

   architecture.set(3);
   architecture[0] = 2;
   architecture[1] = 1;
   architecture[2] = 3;

   nn.set(architecture);
   nn.initialize_parameters(0.0);

   ds.set(5,2,3);
   cee.set(&nn, &ds);
   ds.initialize_data(0.0);

   gradient = cee.calculate_gradient();

   assert_true(gradient.size() == nn.count_parameters_number(), LOG);

   // Test

   nn.set(1, 1);

   nn.initialize_parameters(1.0);
   parameters = nn.arrange_parameters();

   ds.set(2,1,1);
   ds.initialize_data(0.5);

   gradient = cee.calculate_gradient();
   numerical_gradient = nd.calculate_gradient(cee, &CrossEntropyError::calculate_error, parameters);

   std::cout << gradient.to_string() << std::endl;
   std::cout << numerical_gradient.to_string() << std::endl;

   assert_true((gradient - numerical_gradient).calculate_absolute_value() < 1.0e-3, LOG);

   // Test

   nn.set(1, 1);

   nn.randomize_parameters_normal();
   parameters = nn.arrange_parameters();

   ds.set(2,1,1);
   ds.randomize_data_uniform(0.0, 1.0);

   gradient = cee.calculate_gradient();
   numerical_gradient = nd.calculate_gradient(cee, &CrossEntropyError::calculate_error, parameters);

   assert_true((gradient - numerical_gradient).calculate_absolute_value() < 1.0e-3, LOG);
}


void CrossEntropyErrorTest::test_calculate_Hessian(void)
{
   message += "test_calculate_Hessian\n";
}


void CrossEntropyErrorTest::test_to_XML(void)   
{
	message += "test_to_XML\n"; 
}


void CrossEntropyErrorTest::test_from_XML(void)
{
	message += "test_from_XML\n"; 
}


void CrossEntropyErrorTest::run_test_case(void)
{
   // Get methods

   // Set methods

   // Objective methods

   test_calculate_loss();
   test_calculate_selection_loss();

   test_calculate_minimum_loss();

   test_calculate_gradient();
   test_calculate_Hessian();

   // Serialization methods

   test_to_XML();
   test_from_XML();

}


// OpenNN: Open Neural Networks Library.
// Copyright (C) 2005-2016 Roberto Lopez.
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
