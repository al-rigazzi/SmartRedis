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
  function set_data_source_c( client, source_id, source_id_length ) &
    bind(c, name="set_data_source")
    use iso_c_binding, only : c_ptr, c_char, c_bool, c_size_t
    import :: enum_kind
    integer(kind=enum_kind)       :: set_data_source_c
    type(c_ptr),            value :: client
    character(kind=c_char)        :: source_id(*)
    integer(kind=c_size_t), value :: source_id_length
  end function set_data_source_c
end interface

interface
  function use_model_ensemble_prefix_c( client, use_prefix) &
    bind(c, name="use_model_ensemble_prefix")
    use iso_c_binding, only : c_ptr, c_bool
    import :: enum_kind
    integer(kind=enum_kind)       :: use_model_ensemble_prefix_c
    type(c_ptr),            value :: client
    logical(kind=c_bool),   value :: use_prefix
  end function use_model_ensemble_prefix_c
end interface

interface
  function use_tensor_ensemble_prefix_c( client, use_prefix) &
    bind(c, name="use_tensor_ensemble_prefix")
    use iso_c_binding, only : c_ptr, c_bool
    import :: enum_kind
    integer(kind=enum_kind)       :: use_tensor_ensemble_prefix_c
    type(c_ptr),            value :: client
    logical(kind=c_bool),   value :: use_prefix
  end function use_tensor_ensemble_prefix_c
end interface

interface
  function use_dataset_ensemble_prefix_c( client, use_prefix) &
    bind(c, name="use_dataset_ensemble_prefix")
    use iso_c_binding, only : c_ptr, c_bool
    import :: enum_kind
    integer(kind=enum_kind)       :: use_dataset_ensemble_prefix_c
    type(c_ptr),            value :: client
    logical(kind=c_bool),   value :: use_prefix
  end function use_dataset_ensemble_prefix_c
end interface