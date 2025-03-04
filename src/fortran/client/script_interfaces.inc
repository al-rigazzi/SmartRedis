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
  function get_script_c( c_client, key, key_length, script, script_length) bind(c,name="get_script")
    use iso_c_binding, only : c_ptr, c_char, c_size_t
    import :: enum_kind
    integer(kind=enum_kind)                      :: get_script_c
    type(c_ptr), value,            intent(in)    :: c_client   !< Initialized SmartRedis client
    character(kind=c_char),        intent(in)    :: key(*)     !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in)    :: key_length !< The length of the key c-string, excluding null
    type(c_ptr),                   intent(inout) :: script      !< The script as a continuous buffer
    integer(kind=c_size_t),        intent(  out) :: script_length !< Length of the script buffer c-string
  end function get_script_c
end interface

interface
  function set_script_from_file_c( c_client, key, key_length, device, device_length, &
                                     script_file, script_file_length) bind(c,name="set_script_from_file")
    use iso_c_binding, only : c_ptr, c_size_t, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_script_from_file_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: device(*)         !< The name of the device (CPU, GPU, GPU:0, GPU:1...)
    integer(kind=c_size_t), value, intent(in) :: device_length     !< The length of the device name c-string, excluding
                                                                   !! null terminating character
    character(kind=c_char),        intent(in) :: script_file(*)     !< The file storing the script
    integer(kind=c_size_t), value, intent(in) :: script_file_length !< The length of the file name c-string, excluding
                                                                   !! null terminating character
  end function set_script_from_file_c
end interface

interface
  function set_script_from_file_multigpu_c(c_client, key, key_length, script_file, script_file_length, &
                                          first_gpu, num_gpus) bind(c,name="set_script_from_file_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_char, c_int
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_script_from_file_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: script_file(*)     !< The file storing the script
    integer(kind=c_size_t), value, intent(in) :: script_file_length !< The length of the file name c-string, excluding
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to set the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
                                                                   !! null terminating character
  end function set_script_from_file_multigpu_c
end interface

interface
  function set_script_c( c_client, key, key_length, device, device_length, script, script_length ) &
      bind(c, name="set_script")
    use iso_c_binding, only : c_ptr, c_size_t,  c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_script_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: device(*)         !< The name of the device (CPU, GPU, GPU:0, GPU:1...)
    integer(kind=c_size_t), value, intent(in) :: device_length     !< The length of the device name c-string, excluding
                                                                   !! null terminating character
    character(kind=c_char),        intent(in) :: script(*)     !< The file storing the script
    integer(kind=c_size_t), value, intent(in) :: script_length !< The length of the file name c-string, excluding
                                                                   !! null terminating character
  end function set_script_c
end interface

interface
  function set_script_multigpu_c( c_client, key, key_length, script, script_length, & 
                                  first_gpu, num_gpus) bind(c, name="set_script_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_char, c_int
    import :: enum_kind
    integer(kind=enum_kind)                   :: set_script_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client          !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)            !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length        !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: script(*)         !< The file storing the script
    integer(kind=c_size_t), value, intent(in) :: script_length     !< The length of the file name c-string, excluding
                                                                   !! null terminating character
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to set the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
                                                                   !! null terminating character
  end function set_script_multigpu_c
end interface

interface
  function run_script_c(c_client, key, key_length, func, func_length, inputs, input_lengths, n_inputs, &
                         outputs, output_lengths, n_outputs) bind(c, name="run_script")
    use iso_c_binding, only : c_ptr, c_size_t, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: run_script_c
    type(c_ptr), value,            intent(in) :: c_client       !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)         !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length     !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: func(*)        !< The name of the function in the script to call
    integer(kind=c_size_t), value, intent(in) :: func_length    !< The length of the function c-string, excluding
                                                                !! null terminating character
    type(c_ptr),            value, intent(in) :: inputs         !< One or more names of script input nodes (TF scripts)
    type(c_ptr),            value, intent(in) :: input_lengths  !< The length of each input name c-string, excluding
                                                                !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs       !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs        !< One or more names of script output nodes
                                                                !! (TF scripts)
    type(c_ptr),            value, intent(in) :: output_lengths !< The length of each output name c-string,
                                                                !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs      !< The number of outputs

  end function run_script_c
end interface

interface
  function run_script_multigpu_c(c_client, key, key_length, func, func_length, inputs, input_lengths, n_inputs, &
                         outputs, output_lengths, n_outputs, offset, first_gpu, num_gpus)   &
                         bind(c, name="run_script_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_char, c_int
    import :: enum_kind
    integer(kind=enum_kind)                   :: run_script_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client       !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)         !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length     !< The length of the key c-string, excluding null
    character(kind=c_char),        intent(in) :: func(*)        !< The name of the function in the script to call
    integer(kind=c_size_t), value, intent(in) :: func_length    !< The length of the function c-string, excluding
                                                                !! null terminating character
    type(c_ptr),            value, intent(in) :: inputs         !< One or more names of script input nodes (TF scripts)
    type(c_ptr),            value, intent(in) :: input_lengths  !< The length of each input name c-string, excluding
                                                                !! null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_inputs       !< The number of inputs
    type(c_ptr),            value, intent(in) :: outputs        !< One or more names of script output nodes
                                                                !! (TF scripts)
    type(c_ptr),            value, intent(in) :: output_lengths !< The length of each output name c-string,
                                                                !! excluding null terminating character
    integer(kind=c_size_t), value, intent(in) :: n_outputs      !< The number of outputs
    integer(kind=c_int),    value, intent(in) :: offset         !< Index of the current image, such as a processor
    integer(kind=c_int),    value, intent(in) :: first_gpu      !< ID of the first GPU to use the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus       !< How many GPUs to use in the orchestrator
  end function run_script_multigpu_c
end interface

interface
  function delete_script_c(c_client, key, key_length) bind(c, name="delete_script")
    use iso_c_binding, only : c_ptr, c_size_t, c_char
    import :: enum_kind
    integer(kind=enum_kind)                   :: delete_script_c
    type(c_ptr), value,            intent(in) :: c_client       !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)         !< The key to use to place the script
    integer(kind=c_size_t), value, intent(in) :: key_length     !< The length of the key c-string, excluding null
  end function delete_script_c
end interface

interface
  function delete_script_multigpu_c(c_client, key, key_length, first_gpu, num_gpus) &
    bind(c, name="delete_script_multigpu")
    use iso_c_binding, only : c_ptr, c_size_t, c_char, c_int
    import :: enum_kind
    integer(kind=enum_kind)                   :: delete_script_multigpu_c
    type(c_ptr), value,            intent(in) :: c_client       !< Initialized SmartRedis client
    character(kind=c_char),        intent(in) :: key(*)         !< The key to use to place the script_multigpu
    integer(kind=c_size_t), value, intent(in) :: key_length     !< The length of the key c-string, excluding null
    integer(kind=c_int),    value, intent(in) :: first_gpu         !< ID of the first GPU to use the model on
    integer(kind=c_int),    value, intent(in) :: num_gpus          !< How many GPUs to use in the orchestrator
  end function delete_script_multigpu_c
end interface