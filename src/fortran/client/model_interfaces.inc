! BSD 2-Clause License
!
! Copyright (c) 2021-2023, Hewlett Packard Enterprise
! All rights reserved.
!
! Redistribution and use in source and binary forms, with or without
! modification, are permitted provided that the following conditions are met:
!
! 1. Redistributions of source code must retain the above copyright notice, this
!    list of conditions and the following disclaimer.
!
! 2. Redistributions in binary form must reproduce the above copyright notice,
!    this list of conditions and the following disclaimer in the documentation
!    and/or other materials provided with the distribution.
!
! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
! DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
! FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
! DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
! SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
! CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
! OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
! OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

interface
  function get_model_c( c_client, key, key_length, model, model_length, model_data) bind(c,name="get_model")
    use iso_c_binding, only : c_ptr, c_char, c_size_t
    import :: enum_kind
    integer(kind=enum_kind)                      :: get_model_c
    type(c_ptr), value,            intent(in)    :: c_client    !< Initialized SmartRedis client
    character(kind=c_char),        intent(in)    :: key(*)      !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in)    :: key_length  !< The length of the key c-string, excluding null
    type(c_ptr),                   intent(inout) :: model       !< The model as a continuous buffer
    integer(kind=c_size_t),        intent(  out) :: model_length !< Length of the model buffer c-string
    type(c_ptr),                   intent(  out) :: model_data   !< Reeives model information
  end function get_model_c
end interface

interface
  function set_model_from_file_c( c_client, key, key_length, model_file, model_file_length,      &
      backend, backend_length, device, device_length, batch_size, min_batch_size, min_batch_timeout, &
      tag, tag_length, inputs, input_lengths, n_inputs, outputs, output_lengths, n_outputs ) &
      bind(c, name="set_model_from_file")
    use iso_c_binding, only : c_ptr, c_size_t, c_int, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_model_from_file_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: model_file(*)     !< The file storing the model
    integer(kind=c_size_t), value, intent(in) :: model_file_length !< The length of the file name c-string, excluding
                                                                   !! null terminating character
    character(kind=c_char),        intent(in) :: backend(*)        !< The name of the backend (TF, TFLITE, TORCH, ONNX)
    integer(kind=c_size_t), value, intent(in) :: backend_length    !< The length of the backend name c-string,
                                                                   !! excluding null terminating character
    character(kind=c_char),        intent(in) :: device(*)         !< The name of the device (CPU, GPU, GPU:0, GPU:1...)
    integer(kind=c_size_t), value, intent(in) :: device_length     !< The length of the device name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_int),    value, intent(in) :: batch_size        !< The batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_size    !< The minimum batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_timeout !< Max time (ms) to wait for min batch size
    character(kind=c_char),        intent(in) :: tag(*)            !< A tag to attach to the model for information
                                                                   !! purposes
    integer(kind=c_size_t), value, intent(in) :: tag_length        !< The length of the tag c-string, excluding null
                                                                   !! terminating character
    type(c_ptr),            value, intent(in) :: inputs            !< One or more names of model input nodes (TF models)
    type(c_ptr),            value, intent(in) :: input_lengths     !< The length of each input name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs          !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs           !< One or more names of model output nodes
                                                                   !! (TF models)
    type(c_ptr),            value, intent(in) :: output_lengths    !< The length of each output name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs         !< The number of outputs
  end function set_model_from_file_c
end interface

interface
  function set_model_from_file_multigpu_c( c_client, key, key_length, model_file, model_file_length,      &
      backend, backend_length, first_gpu, num_gpus, batch_size, min_batch_size, min_batch_timeout, &
      tag, tag_length, inputs, input_lengths, n_inputs, outputs, output_lengths, n_outputs) &
      bind(c, name="set_model_from_file_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_int, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_model_from_file_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: model_file(*)     !< The file storing the model
    integer(kind=c_size_t), value, intent(in) :: model_file_length !< The length of the file name c-string, excluding
                                                                   !! null terminating character
    character(kind=c_char),        intent(in) :: backend(*)        !< The name of the backend (TF, TFLITE, TORCH, ONNX)
    integer(kind=c_size_t), value, intent(in) :: backend_length    !< The length of the backend name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to set the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
                                                                   !! null terminating character
    integer(kind=c_int),    value, intent(in) :: batch_size        !< The batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_size    !< The minimum batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_timeout !< Max time (ms) to wait for min batch size
    character(kind=c_char),        intent(in) :: tag(*)            !< A tag to attach to the model for information
                                                                   !! purposes
    integer(kind=c_size_t), value, intent(in) :: tag_length        !< The length of the tag c-string, excluding null
                                                                   !! terminating character
    type(c_ptr),            value, intent(in) :: inputs            !< One or more names of model input nodes (TF models)
    type(c_ptr),            value, intent(in) :: input_lengths     !< The length of each input name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs          !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs           !< One or more names of model output nodes
                                                                   !! (TF models)
    type(c_ptr),            value, intent(in) :: output_lengths    !< The length of each output name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs         !< The number of outputs
  end function set_model_from_file_multigpu_c
end interface

interface
  function set_model_c( c_client, key, key_length, model, model_length,      &
      backend, backend_length, device, device_length, batch_size, min_batch_size, min_batch_timeout, &
      tag, tag_length, inputs, input_lengths, n_inputs, outputs, output_lengths, n_outputs) &
      bind(c, name="set_model")
    use iso_c_binding, only : c_ptr, c_size_t, c_int, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_model_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: model(*)          !< The file storing the model
    integer(kind=c_size_t), value, intent(in) :: model_length      !< The length of the file name c-string, excluding
                                                                   !! null terminating character
    character(kind=c_char),        intent(in) :: backend(*)        !< The name of the backend (TF, TFLITE, TORCH, ONNX)
    integer(kind=c_size_t), value, intent(in) :: backend_length    !< The length of the backend name c-string,
                                                                   !! excluding null terminating character
    character(kind=c_char),        intent(in) :: device(*)         !< The name of the device (CPU, GPU, GPU:0, GPU:1...)
    integer(kind=c_size_t), value, intent(in) :: device_length     !< The length of the device name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_int),    value, intent(in) :: batch_size        !< The batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_size    !< The minimum batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_timeout !< Max time (ms) to wait for min batch size
    character(kind=c_char),        intent(in) :: tag(*)            !< A tag to attach to the model for information
                                                                   !! purposes
    integer(kind=c_size_t), value, intent(in) :: tag_length        !< The length of the tag c-string, excluding null
                                                                   !! terminating character
    type(c_ptr),            value, intent(in) :: inputs            !< One or more names of model input nodes (TF models)
    type(c_ptr),            value, intent(in) :: input_lengths     !< The length of each input name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs          !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs           !< One or more names of model output nodes
                                                                   !! (TF models)
    type(c_ptr),            value, intent(in) :: output_lengths    !< The length of each output name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs         !< The number of outputs
  end function set_model_c
end interface

interface
  function set_model_multigpu_c( c_client, key, key_length, model, model_length,      &
      backend, backend_length, first_gpu, num_gpus, batch_size, min_batch_size, min_batch_timeout, &
      tag, tag_length, inputs, input_lengths, n_inputs, outputs, output_lengths, n_outputs) &
      bind(c, name="set_model_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_int, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_model_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: model(*)          !< The file storing the model
    integer(kind=c_size_t), value, intent(in) :: model_length      !< The length of the file name c-string, excluding
                                                                   !! null terminating character
    character(kind=c_char),        intent(in) :: backend(*)        !< The name of the backend (TF, TFLITE, TORCH, ONNX)
    integer(kind=c_size_t), value, intent(in) :: backend_length    !< The length of the backend name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to set the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
                                                                   !! null terminating character
    integer(kind=c_int),    value, intent(in) :: batch_size        !< The batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_size    !< The minimum batch size for model execution
    integer(kind=c_int),    value, intent(in) :: min_batch_timeout !< Max time (ms) to wait for min batch size
    character(kind=c_char),        intent(in) :: tag(*)            !< A tag to attach to the model for information
                                                                   !! purposes
    integer(kind=c_size_t), value, intent(in) :: tag_length        !< The length of the tag c-string, excluding null
                                                                   !! terminating character
    type(c_ptr),            value, intent(in) :: inputs            !< One or more names of model input nodes (TF models)
    type(c_ptr),            value, intent(in) :: input_lengths     !< The length of each input name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs          !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs           !< One or more names of model output nodes
                                                                   !! (TF models)
    type(c_ptr),            value, intent(in) :: output_lengths    !< The length of each output name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs         !< The number of outputs
  end function set_model_multigpu_c
end interface

interface
  function run_model_c(c_client, key, key_length, inputs, input_lengths, n_inputs, &
                         outputs, output_lengths, n_outputs) bind(c, name="run_model")
    use iso_c_binding, only : c_ptr, c_size_t, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: run_model_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    type(c_ptr),            value, intent(in) :: inputs            !< One or more names of model input nodes (TF models)
    type(c_ptr),            value, intent(in) :: input_lengths     !< The length of each input name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs          !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs           !< One or more names of model output nodes
                                                                   !! (TF models)
    type(c_ptr),            value, intent(in) :: output_lengths    !< The length of each output name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs         !< The number of outputs
  end function run_model_c
end interface

interface
  function run_model_multigpu_c(c_client, key, key_length, inputs, input_lengths, n_inputs, &
                         outputs, output_lengths, n_outputs, offset, first_gpu, num_gpus)   &
                         bind(c, name="run_model_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_char, c_int
    import :: enum_kind
    integer(kind=enum_kind)                   :: run_model_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    type(c_ptr),            value, intent(in) :: inputs            !< One or more names of model input nodes (TF models)
    type(c_ptr),            value, intent(in) :: input_lengths     !< The length of each input name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs          !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs           !< One or more names of model output nodes
                                                                   !! (TF models)
    type(c_ptr),            value, intent(in) :: output_lengths    !< The length of each output name c-string,
                                                                   !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs         !< The number of outputs
    integer(kind=c_int),    value, intent(in) :: offset            !< Index of the current image, such as a processor
                                                                   !! ID or MPI rank
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to use the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
  end function run_model_multigpu_c
end interface

interface
  function delete_model_c(c_client, key, key_length) bind(c, name="delete_model")
    use iso_c_binding, only : c_ptr, c_size_t, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: delete_model_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to delete the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
  end function delete_model_c
end interface

interface
  function delete_model_multigpu_c(c_client, key, key_length, first_gpu, num_gpus) bind(c, name="delete_model_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_char, c_int
    import :: enum_kind
    integer(kind=enum_kind)                   :: delete_model_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to delete the model
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to use the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
  end function delete_model_multigpu_c
end interface
