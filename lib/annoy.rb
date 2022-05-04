# frozen_string_literal: true

require 'annoy/version'
require 'annoy/annoyext'

module Annoy
  # AnnoyIndex is a class that provides functions for k-nearest neighbors search.
  # The methods in this class are implemented similarly to Annoy's Python API (https://github.com/spotify/annoy#full-python-api).
  #
  # @example
  #   require 'annoy'
  #
  #   index = AnnoyIndex.new(n_features: 100, metric: 'euclidean')
  #
  #   5000.times do |item_id|
  #     item_vec = Array.new(100) { rand - 0.5 }
  #     index.add_item(item_id, item_vec)
  #   end
  #
  #   index.build(10)
  #
  #   index.get_nns_by_item(0, 100)
  #
  class AnnoyIndex
    # Returns the number of features of indexed item.
    # @return [Integer]
    attr_reader :n_features

    # Returns the metric of index.
    # @return [String]
    attr_reader :metric

    # Returns the data type of feature.
    # @return [String]
    attr_reader :dtype

    # Create a new search index.
    #
    # @param n_features [Integer] The number of features (dimensions) of stored vector.
    # @param metric [String] The distance metric between vectors ('angular', 'dot', 'hamming', 'euclidean', or 'manhattan').
    # @param dtype [String] The data type of features ('float64' and 'float32').
    #   If metric is given 'hamming', 'uint64' is automatically assigned to this argument.
    def initialize(n_features:, metric: 'angular', dtype: 'float64') # rubocop:disable Metrics/AbcSize, Metrics/CyclomaticComplexity, Metrics/MethodLength
      raise ArgumentError, 'Expect n_features to be Integer.' unless n_features.is_a?(Numeric)

      @n_features = n_features.to_i
      @metric = metric
      @dtype = dtype

      # rubocop:disable Layout/LineLength
      @index = case @metric
               when 'angular'
                 @dtype == 'float64' ? AnnoyIndexAngular.new(@n_features) : AnnoyIndexAngularFloat32.new(@n_features)
               when 'dot'
                 @dtype == 'float64' ? AnnoyIndexDotProduct.new(@n_features) : AnnoyIndexDotProductFloat32.new(@n_features)
               when 'hamming'
                 @dtype = 'uint64'
                 AnnoyIndexHamming.new(@n_features)
               when 'euclidean'
                 @dtype == 'float64' ? AnnoyIndexEuclidean.new(@n_features) : AnnoyIndexEuclideanFloat32.new(@n_features)
               when 'manhattan'
                 @dtype == 'float64' ? AnnoyIndexManhattan.new(@n_features) : AnnoyIndexManhattanFloat32.new(@n_features)
               else
                 raise ArgumentError, "No such metric: #{@metric}."
               end
      # rubocop:enable Layout/LineLength
    end

    # Add item to be indexed.
    #
    # @param i [Integer] The ID of item.
    # @param v [Array] The vector of item.
    # @return [Boolean]
    def add_item(i, v)
      @index.add_item(i, v)
    end

    # Build a forest of index trees. After building, no more items can be added.
    #
    # @param n_trees [Integer] The number of trees. More trees gives higher search precision.
    # @param n_jobs [Integer] The number of threads used to build the trees. If -1 is given, uses all available CPU cores.
    # This parameter is enabled only if "-DANNOYLIB_MULTITHREADED_BUILD" is specified on gem installation.
    # @return [Boolean]
    def build(n_trees, n_jobs: -1)
      @index.build(n_trees, n_jobs)
    end

    # Save the search index to disk. After saving, no more items can be added.
    #
    # @param filename [String] The filename of search index.
    # @return [Boolean]
    def save(filename, prefault: false)
      @index.save(filename, prefault)
    end

    # Load a search index from disk.
    #
    # @param filename [String] The filename of search index.
    # @param prefault [Boolean] The flag indicating whether to pre-read the entire file into memory.
    # @return [Boolean]
    def load(filename, prefault: false)
      @index.load(filename, prefault)
    end

    # Unload the search index.
    #
    # @return [Boolean]
    def unload
      @index.unload
    end

    # Search the n closest items.
    #
    # @param i [Integer] The ID of query item.
    # @param n [Integer] The number of nearest neighbors.
    # @param search_k [Integer] The maximum number of nodes inspected during the search. If -1 is given, it sets to n * n_trees.
    # @param include_distances [Boolean] The flag indicating whether to returns all corresponding distances.
    # @return [Array<Integer> or Array<Array<Integer>, Array<Float>>]
    def get_nns_by_item(i, n, search_k: -1, include_distances: false)
      @index.get_nns_by_item(i, n, search_k, include_distances)
    end

    # Search the n closest items.
    #
    # @param v [Array] The vector of query item.
    # @param n [Integer] The number of nearest neighbors.
    # @param search_k [Integer] The maximum number of nodes inspected during the search. If -1 is given, it sets to n * n_trees.
    # @param include_distances [Boolean] The flag indicating whether to returns all corresponding distances.
    # @return [Array<Integer> or Array<Array<Integer>, Array<Float>>]
    def get_nns_by_vector(v, n, search_k: -1, include_distances: false)
      @index.get_nns_by_vector(v, n, search_k, include_distances)
    end

    # Return the item vector.
    #
    # @param i [Integer] The ID of item.
    # @return [Array]
    def get_item(i)
      @index.get_item(i)
    end

    # Calculate the distances between items.
    #
    # @param i [Integer] The ID of item.
    # @param j [Integer] The ID of item.
    # @return [Float or Integer]
    def get_distance(i, j)
      @index.get_distance(i, j)
    end

    # Return the number of items in the search index.
    # @return [Integer]
    def n_items
      @index.get_n_items
    end

    # Return the number of trees in the search index.
    # @return [Integer]
    def n_trees
      @index.get_n_trees
    end

    # Prepare annoy to build the index in the specified file instead of RAM.
    # (call this method before adding items, no need to save after building).
    #
    # @param filename [String] The filename of search index.
    # @return [Boolean]
    def on_disk_build(filename)
      @index.on_disk_build(filename)
    end

    # Set to verbose mode.
    #
    # @param flag [Boolean]
    def verbose(flag)
      @index.verbose(flag)
    end

    # Set seed for the random number generator.
    #
    # @param s [Integer]
    def seed(s)
      @index.set_seed(s)
    end
  end
end
