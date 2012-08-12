=head1 NAME

gamma - gamma distribution.

=head1 SYNOPSIS

    x ~ gamma()
    x ~ gamma(2.0, 5.0)
    x ~ gamma(shape = 2.0, scale = 5.0)

=head1 DESCRIPTION

A C<gamma> action specifies a variate that is gamma distributed according to
the given C<shape> and C<scale> parameters.

=cut

package Bi::Action::gamma;

use base 'Bi::Model::Action';
use warnings;
use strict;

=head1 PARAMETERS

=over 4

=item * C<shape> (position 0, default 1.0)

Shape parameter of the distribution.

=item * C<upper> (position 1, default 1.0)

Scale parameter of the distribution.

=back

=cut
our $ACTION_ARGS = [
  {
    name => 'shape',
    positional => 1,
    default => 1.0
  },
  {
    name => 'scale',
    positional => 1,
    default => 1.0
  }
];

sub validate {
    my $self = shift;
    
    $self->process_args($ACTION_ARGS);
    $self->ensure_op('~');
    $self->ensure_scalar('shape');
    $self->ensure_scalar('scale');
    $self->set_parent('gamma_');
    $self->set_can_combine(1);
    $self->set_unroll_args(0);
}

sub mean {
    my $self = shift;

    # shape*scale    
    my $shape = $self->get_named_arg('shape')->clone;
    my $scale = $self->get_named_arg('scale')->clone;
    my $mean = $shape*$scale;

    return $mean;
}

sub jacobian {
    my $self = shift;
    
    my $mean = $self->mean;
    my @refs = @{$mean->get_vars};
    my @J = map { $mean->d($_) } @refs;

    return (\@J, \@refs);
}

1;

=head1 AUTHOR

Lawrence Murray <lawrence.murray@csiro.au>

=head1 VERSION

$Rev$ $Date$
