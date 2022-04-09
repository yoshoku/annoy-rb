# annoy-rb

[![Build Status](https://github.com/yoshoku/annoy.rb/workflows/build/badge.svg)](https://github.com/yoshoku/annoy.rb/actions?query=workflow%3Abuild)
[![Gem Version](https://badge.fury.io/rb/annoy-rb.svg)](https://badge.fury.io/rb/annoy-rb)
[![License](https://img.shields.io/badge/License-Apache%202.0-yellowgreen.svg)](https://github.com/yoshoku/annoy.rb/blob/main/LICENSE.txt)
[![Documentation](https://img.shields.io/badge/api-reference-blue.svg)](https://yoshoku.github.io/annoy.rb/doc/)

annoy-rb provides Ruby bindings for the [Annoy (Approximate Nearest Neighbors Oh Yeah)](https://github.com/spotify/annoy).

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'annoy-rb'
```

And then execute:

    $ bundle install

Or install it yourself as:

    $ gem install annoy-rb

Note: annoy-rb does not require the installation of another external library.
In addition, annoy-rb does not give any optimization options when building native extensions.
If necessary, add optimization options yourself during installation, as follows;

```
$ bundle config --local build.annoy-rb "--with-cxxflags=-march=native"
$ bundle install
```

Or:

```
$ gem install annoy-rb -- --with-cxxflags=-march=native
```

## Documentation

* [annoy-rb API Documentation](https://yoshoku.github.io/annoy.rb/doc/)

## Usage

```ruby
require 'annoy'

f = 40 # length of item vector that will be indexed.
t = Annoy::AnnoyIndex.new(n_features: f, metric: 'angular')

1000.times do |i|
  v = Array.new(f) { rand }
  t.add_item(i, v)
end

t.build(10) # 10 trees.
t.save('test.ann')

u = Annoy::AnnoyIndex.new(n_features: f, metric: 'angular')
u.load('test.ann')
p u.get_nns_by_item(0, 100) # will find the 100 nearest neighbors.
```

With the default argument, annoy-rb uses double precision floating point type for the data type of vector element.
On the other hand, the [Python bindings of Annoy](https://pypi.org/project/annoy/) use single precision floating point type.
If you want to load a search index created with the Python bindings, specify 'float32' to the dtype argument.

```ruby
require 'annoy'

f = 40
t = Annoy::AnnoyIndex.new(n_features: f, metric: 'angular', dtype: 'float32')
t.load('index_with_python_bindings.ann')
```

## License

The gem is available as open source under the terms of the [Apache-2.0 License](https://www.apache.org/licenses/LICENSE-2.0).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/yoshoku/annoy.rb.
This project is intended to be a safe, welcoming space for collaboration,
and contributors are expected to adhere to the [Contributor Covenant](https://contributor-covenant.org) code of conduct.
